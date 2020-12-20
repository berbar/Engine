#pragma once

#include <opencv2/core/mat.hpp>
#include <iberbar/Poster/MathTypes.h>

namespace iberbar
{
	namespace Poster
	{
		// ��ȫ����Mat�����߽磬���׳��쳣
		void SafeDrawMat( cv::Mat matSrc, cv::Mat matDest, const CRect2i& rcDest, const CColor4B* color = nullptr );

		// ��ȫ����Mat�����߽磬���׳��쳣
		void SafeDrawMat( cv::Mat matSrc, const CRect2i& rcSrc, cv::Mat matDest, const CRect2i& rcDest, const CColor4B* color = nullptr );

		cv::Mat BuildMaskMat_Circle( int radius );

		cv::Mat BuildMaskMat_RectangleWithCorners( const CSize2i& size, int cornerRadius );
	}
}


