#pragma once

#include <iberbar/Utility/Unknown.h>
#include <iberbar/Poster/MathTypes.h>

namespace iberbar
{
	namespace Poster
	{
		struct __iberbarExports__ ULineRenderOptions
		{
		public:
			int width;
			int style;
		};

		class CTextureScale9;
		class CSurface;

		/*
			���ƾŹ���ͼƬ
		*/
		void __iberbarExports__ DrawTextureScale9( CSurface* surface, const CRect2i& dest, CTextureScale9* texture );

		/*
			����ͼƬ��һ������������
		*/
		void __iberbarExports__ DrawTexture( CSurface* surface, const CRect2i& dest, CSurface* texture, __maybenull const CRect2i* src = nullptr );

		void __iberbarExports__ DrawTexture( CSurface* surface, const CRect2i& dest, CSurface* texture, const CColor4B& color, __maybenull const CRect2i* src = nullptr );

		/*
			����ͼƬ��һ��Բ��������
		*/
		void __iberbarExports__ DrawTextureInCircle( CSurface* surface, const CCircle& dest, CSurface* texture, __maybenull const CRect2i* src = nullptr );

		/*
			����ͼƬ��һ������Բ�ǵľ���������
		*/
		void __iberbarExports__ DrawTextureInCornersBox( CSurface* surface, const CRect2i& dest, int radius, CSurface* texture, __maybenull const CRect2i* src = nullptr );

		/*
			��������
		*/
		void __iberbarExports__ DrawLine( CSurface* surface, const CPoint2i* points, int pointsCount, const CColor4B& color, const ULineRenderOptions* options = nullptr );

		void __iberbarExports__ DrawLine2( CSurface* surface, const CPoint2i& point1, const CPoint2i& point2, const CColor4B& color, const ULineRenderOptions* options = nullptr );

		/*
			���ƾ���
			@params thickness �߿�-1ʱ���������������
		*/
		void __iberbarExports__ DrawRectagle( CSurface* surface, const CRect2i& rect, const CColor4B& color, int thickness );



	}
}

