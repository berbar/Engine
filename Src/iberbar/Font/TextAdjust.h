#pragma once

#include <iberbar/Utility/String.h>
#include <iberbar/Utility/Rect.h>
#include <iberbar/Font/FontBase.h>

namespace iberbar
{
	struct UTextLineInfo
	{
		const wchar_t* ptr;
		int width;
		int charCount;
	};

	struct UTextAdjustResult
	{
		std::vector<UTextLineInfo> lines;
		bool overflow;

		UTextAdjustResult()
			: lines()
			, overflow( false )
		{
		}
	};
	
	class TTextAdjustSolution
	{
	public:
		enum class UCharType
		{
			EngLetter,
			WhiteSpace,
			Else,
			None
		};

	public:
		TTextAdjustSolution();

	public:
		template < typename TFont >
		void Adjust( TFont* font, const wchar_t* ptr, CRect2i* pRect );

		template < typename TFont >
		void Adjust( TFont* font, const wchar_t* ptr, CRect2i* pRect, int maxLine, const wchar_t* ellipsisText );

		const UTextAdjustResult& GetLines() { return m_result; }

	private:
		void Reset();

		void AddChar_EngLetter( wchar_t character, int width );
		void AddChar_Else( wchar_t character, int width );
		void AddChar_EndLine();
		void AddChar_WhiteSpace( int width );
		void Wrap();

		// �ж��Ƿ�ǰ�еĵ�һ�����ʻ��ַ�
		bool IsFirstWordAtLine();

		//int GetCurrentWordCharCount();
		// ������ǰ���ʲ����ص��ʵ��ַ���
		int PopCurrentEngWord();

		template < typename TFont >
		int MeasureCharWidth( TFont* font, int fontSizeDef, wchar_t charater );

	private:
		int m_lineWordCount;   // ��ǰ�е��׸���
		int m_lineWidth;   // ��ǰ�еĿ��
		int m_lineCharCount;   // ��ǰ�е��ַ���

		UCharType m_wordCharType;
		int m_wordCharWidth;
		int m_wordCharCount;

		int m_lineCount;
		//std::vector< UTextLineInfo > m_lines;
		UTextAdjustResult m_result;
		//CRect2i m_rcRender;
	};
}


inline iberbar::TTextAdjustSolution::TTextAdjustSolution()
	: m_lineWordCount( 0 )
	, m_lineWidth( 0 )
	, m_lineCharCount( 0 )
	, m_wordCharType( UCharType::None )
	, m_wordCharWidth( 0 )
	, m_wordCharCount( 0 )
	, m_lineCount( 0 )
	, m_result()
{
}

FORCEINLINE void iberbar::TTextAdjustSolution::Reset()
{
	m_lineWordCount = 0;
	m_lineWidth = 0;
	m_lineCharCount = 0;
	m_wordCharType = UCharType::None;
	m_wordCharWidth = 0;
	m_wordCharCount = 0;
	m_lineCount = 0;
	m_result = UTextAdjustResult();
}

FORCEINLINE void iberbar::TTextAdjustSolution::AddChar_EngLetter( wchar_t character, int width )
{
	if ( m_wordCharType != UCharType::EngLetter )
	{
		m_wordCharCount = 0;
		m_wordCharWidth = 0;

		m_lineWordCount++;
	}
	m_wordCharType = UCharType::EngLetter;
	m_wordCharCount++;
	m_wordCharWidth += width;
	m_lineCharCount++;
}

FORCEINLINE void iberbar::TTextAdjustSolution::AddChar_Else( wchar_t character, int width )
{
	// ���ĺͱ��ȷ��Ų���ɴ���
	m_wordCharType = UCharType::Else;

	// ����һ���ַ�Ϊһ����
	m_lineWordCount++;

	m_lineWidth += width;
	m_lineCharCount++;
}

FORCEINLINE void iberbar::TTextAdjustSolution::AddChar_WhiteSpace( int width )
{
	m_wordCharType = UCharType::WhiteSpace;

	m_lineWidth += width;
	m_lineCharCount++; //�ո�Ҳ��Ҫ�����ַ�����
}

FORCEINLINE void iberbar::TTextAdjustSolution::AddChar_EndLine()
{
	m_lineCharCount++;
	Wrap();
}

FORCEINLINE void iberbar::TTextAdjustSolution::Wrap()
{
	m_lineCount++;

	UTextLineInfo lineInfo;
	lineInfo.width = m_lineWidth;
	lineInfo.charCount = m_lineCharCount;
	m_result.lines.push_back( lineInfo );

	m_lineWidth = 0;
	m_lineCharCount = 0;
	m_lineWordCount = 0;
	m_wordCharCount = 0;
	m_wordCharWidth = 0;
	m_wordCharType = UCharType::None;
}

FORCEINLINE int iberbar::TTextAdjustSolution::PopCurrentEngWord()
{
	m_lineCharCount -= m_wordCharCount;
	m_lineWidth -= m_wordCharWidth;
	return m_wordCharCount;
}

FORCEINLINE bool iberbar::TTextAdjustSolution::IsFirstWordAtLine()
{
	return m_lineWordCount == 1;
}

template < typename TFont >
FORCEINLINE int iberbar::TTextAdjustSolution::MeasureCharWidth( TFont* font, int fontSizeDef, wchar_t charater )
{
	auto bitmap = font->GetCharBitmap( charater );
	if ( bitmap == nullptr )
		return fontSizeDef;
	return bitmap->GetCharWidth();
}


template < typename TFont >
void iberbar::TTextAdjustSolution::Adjust( TFont* font, const wchar_t* ptr, iberbar::CRect2i* pRect )
{
	// ����
	Reset();

	const UFontDesc& fontDesc = font->GetFontDesc();
	int fontSize = fontDesc.Size;

	int charWidth = 0;
	int rectWidth = pRect->Width();

	int charWidth_WhiteSpace = this->MeasureCharWidth( font, fontSize, L' ' );

	while ( ptr[0] )
	{
		if ( IsWrap_Char32( *ptr ) )
		{
			AddChar_EndLine();
		}
		else
		{
			charWidth = this->MeasureCharWidth<TFont>( font, fontSize, *ptr );

			// �����е��ַ������߽�ʱ
			if ( (m_lineWidth + charWidth) > rectWidth )
			{
				// �����ǰ�ַ�ΪӢ��
				if ( IsEngLetter_Char32( *ptr ) )
				{
					// ���һ��Ӣ�ĵ��ʿ�ȴ���rect��ȶ����ǵ�һ�����ʣ���д���ٻ���
					if ( IsFirstWordAtLine() == true )
					{
						while ( IsEngLetter_Char32( *ptr ) )
						{
							charWidth = this->MeasureCharWidth<TFont>( font, fontSize, *ptr );
							AddChar_EngLetter( *ptr, charWidth );
							ptr++;
						}

						while ( Char32_IsWhiteSpace( *ptr ) )
						{
							AddChar_WhiteSpace( charWidth_WhiteSpace );
							ptr++;
						}

						if ( *ptr == L'\n' )
						{
							AddChar_EndLine();
						}
					}
					else
					{
						// �������ʣ��α�ص�����ǰ��
						ptr -= PopCurrentEngWord() + 1;
					}

					//lc_Tag.changeLine();
					Wrap();
				}
				// �����ǰ�ַ�Ϊ�ո�
				else if ( ptr[0] == L' ' )
				{
					// �ո��������Χ�����У�ֱ���ո����
					while ( Char32_IsWhiteSpace( *ptr ) )
					{
						AddChar_WhiteSpace( charWidth_WhiteSpace );
						ptr++;
					}

					if ( *ptr == L'\n' )
					{
						AddChar_EndLine();
					}

					Wrap();
				}
				// �����ǰ�ַ�Ϊ�����ַ�
				else
				{
					// ���ĺͱ��ȷ��Ų���ɴ���
					Wrap();
					ptr--;
				}
			}

			// û�г����߽�
			else
			{
				if ( IsEngLetter_Char32( *ptr ) )
				{
					// ���ӳ�Ӣ�ĵ���
					AddChar_EngLetter( *ptr, charWidth );
				}
				// �����ǰ�ַ�Ϊ�ո�
				else if ( Char32_IsWhiteSpace( *ptr ) )
				{
					AddChar_WhiteSpace( charWidth_WhiteSpace );
				}
				// �����ǰ�ַ�Ϊ�����ַ�
				else
				{
					AddChar_Else( *ptr, charWidth );
				}
			}
		}

		ptr++;
	} // end while

	//-------------------------------------------
	//  ���һ��
	//-------------------------------------------
	Wrap();
}

template < typename TFont >
void iberbar::TTextAdjustSolution::Adjust( TFont* font, const wchar_t* ptr, iberbar::CRect2i* pRect, int maxLine, const wchar_t* ellipsisText )
{
	// ����
	Reset();

	const UFontDesc& fontDesc = font->GetFontDesc();
	int fontSize = fontDesc.Size;

	int charWidth = 0;
	int rectWidth = pRect->Width();
	int lineMaxWidth = rectWidth;
	maxLine = ( maxLine > 0 ) ? maxLine : INT_MAX;
	int lineMaxIndex = maxLine - 1;

	// ��ȡ�������ĳ���
	int ellipsisWidth = 0;
	if ( maxLine > 0 )
	{
		const wchar_t* ellipsisPtr = ellipsisText;
		while ( *ellipsisPtr != 0 )
		{
			ellipsisWidth += this->MeasureCharWidth( font, fontSize, *ellipsisPtr );
			ellipsisPtr++;
		}
	}

	if ( lineMaxIndex == 0 )
	{
		lineMaxWidth = rectWidth - ellipsisWidth;
	}

	int charWidth_WhiteSpace = this->MeasureCharWidth( font, fontSize, L' ' );
	
	while ( ptr[0] )
	{
		if ( IsWrap_Char32( *ptr ) )
		{
			AddChar_EndLine();

			if ( m_lineCount >= maxLine )
			{
				m_result.overflow = true;
				break;
			}
		}
		else
		{
			charWidth = this->MeasureCharWidth<TFont>( font, fontSize, *ptr );

			// �����е��ַ������߽�ʱ
			if ( (m_lineWidth + charWidth) > lineMaxWidth )
			{
				// �����ǰ�ַ�ΪӢ��
				if ( IsEngLetter_Char32( *ptr ) )
				{
					// ���һ��Ӣ�ĵ��ʿ�ȴ���rect��ȶ����ǵ�һ�����ʣ���д���ٻ���
					if ( IsFirstWordAtLine() == true )
					{
						while ( IsEngLetter_Char32( *ptr ) )
						{
							charWidth = this->MeasureCharWidth<TFont>( font, fontSize, *ptr );
							AddChar_EngLetter( *ptr, charWidth );
							ptr++;
						}

						while ( Char32_IsWhiteSpace( *ptr ) )
						{
							AddChar_WhiteSpace( charWidth_WhiteSpace );
							ptr++;
						}

						if ( *ptr == L'\n' )
						{
							AddChar_EndLine();
						}
						else
						{
							Wrap();
						}
					}
					else
					{
						// �������ʣ��α�ص�����ǰ��
						ptr -= PopCurrentEngWord() + 1;
						Wrap();
					}
				}
				// �����ǰ�ַ�Ϊ�ո�
				else if ( ptr[0] == L' ' )
				{
					// �ո��������Χ�����У�ֱ���ո����
					while ( Char32_IsWhiteSpace( *ptr ) )
					{
						AddChar_WhiteSpace( charWidth_WhiteSpace );
						ptr++;
					}

					if ( *ptr == L'\n' )
					{
						AddChar_EndLine();
					}
					else
					{
						Wrap();
					}
				}
				// �����ǰ�ַ�Ϊ�����ַ�
				else
				{
					// ���ĺͱ��ȷ��Ų���ɴ���
					Wrap();
					ptr--;
				}

				if ( m_lineCount >= maxLine )
				{
					m_result.overflow = true;
					break;
				}
			}

			// û�г����߽�
			else
			{
				if ( IsEngLetter_Char32( *ptr ) )
				{
					// ���ӳ�Ӣ�ĵ���
					AddChar_EngLetter( *ptr, charWidth );
				}
				// �����ǰ�ַ�Ϊ�ո�
				else if ( Char32_IsWhiteSpace( *ptr ) )
				{
					AddChar_WhiteSpace( charWidth_WhiteSpace );
				}
				// �����ǰ�ַ�Ϊ�����ַ�
				else
				{
					AddChar_Else( *ptr, charWidth );
				}
			}
		}

		ptr++;
	} // end while

	if ( m_result.overflow == true )
	{

	}

	//-------------------------------------------
	//  ���һ��
	//-------------------------------------------
	Wrap();
}
