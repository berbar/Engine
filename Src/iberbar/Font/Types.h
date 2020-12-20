#pragma once

#include <iberbar/Font/Headers.h>

#undef DrawText
#undef GetCharWidth

namespace iberbar
{
	enum class UFontBitsFormat
	{
		ARGB,
		BGRA,
		RGBA
	};

	struct UFontDesc
	{
		//int Height;
		//int Width;
		int Size;
		int Weight;
		int Italic;
		//uint8 CharSet;
		//uint8 OutputPrecision;
		//uint8 Quality;
		//uint8 PitchAndFamily;
		std::string FamilyName;// �������ļ�
	};


	namespace TextDraw
	{
		// �ı����з�ʽ
		enum class UFontDrawTextWorkBreak
		{
			// �Ӳ�����
			NoBreak,

			// ֻ�л��з��Ż���
			Hyphenate,

			// ���з��������߽�ʱ����
			BreakAll
		};
	}



	// �ı����ʱ�Ĵ���ʽ
	enum class UFontDrawTextOverflow
	{
		// �������
		Unset,

		// �ü�����ȫ������ַ�������Ⱦ����������ַ�ֻ��Ⱦ�������ڵĲ���
		Clip,

		// ������ַ�����Ⱦ����β���ض��ı�����
		Ellipsis,

		// ���أ���ȫ����Ͱ�������ַ�������Ⱦ
		Hidden
	};

	struct UFontDrawTextSpecialStyle
	{
		int nStart;
		int nCount;
		CColor4B nColor;
	};




	namespace TextDraw
	{
		struct UFontDrawTextOptions
		{
		public:
			typedef const UFontDrawTextSpecialStyle _SpecialStyle;
		public:
			UFontDrawTextOptions()
				: alignHorizental( UAlignHorizental::Left )
				, alignVertical( UAlignVertical::Top )
				, nWrapType( UFontDrawTextWorkBreak::NoBreak )
				, overflow( UFontDrawTextOverflow::Clip )
				, calculateRect( false )
				, bRenderText( true )
				, lineHeight( 0 )
				, ellipsis( nullptr )
				, maxLine( 0 )
				, pStyleSelected( nullptr )
			{
			}

			UAlignHorizental alignHorizental; // ˮƽ����
			UAlignVertical alignVertical; // ��ֱ����
			UFontDrawTextWorkBreak nWrapType; // ���з�ʽ
			UFontDrawTextOverflow overflow; // �ı��Ƴ�����ʽ
			bool calculateRect; // �Ƿ������ƺ�ľ�������
			bool bRenderText;
			int lineHeight; // �и�
			const wchar_t* ellipsis;
			int maxLine; // ����������ƣ���������ĩβ����ellipsisչʾ
			const UFontDrawTextSpecialStyle* pStyleSelected;
		};


		template < typename TFont >
		struct TContextBeginDrawCharBitmaps
		{
			const TFont* pFont;
			const CRect2i* pRcViewport;
			const wchar_t* strText;
			int nCount;
			CRect2i Rect;
			CColor4B FillColor;
			const UFontDrawTextOptions* pOptions;

			TContextBeginDrawCharBitmaps()
				: pFont( nullptr )
				, pRcViewport( nullptr )
				, strText( nullptr )
				, nCount( 0 )
				, Rect()
				, FillColor()
				, pOptions( nullptr )
			{
			}
		};


		template < typename TFont, typename TFontCharBitmap >
		struct TContextDrawCharBitmap
		{
			wchar_t nChar;
			int nIndexAtText;
			int nLine;
			TFont* pFont;
			const TFontCharBitmap* pBitmap;
			const CPoint2i* pPointDrawAt;
			CColor4B FillColor;
			const CRect2i* pRcViewport;
		};
	}

	typedef TextDraw::UFontDrawTextOptions UFontDrawTextOptions;
	typedef TextDraw::UFontDrawTextWorkBreak UFontDrawTextWorkBreak;
}