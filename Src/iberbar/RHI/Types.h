#pragma once

#include <iberbar/RHI/Headers.h>

namespace iberbar
{
    namespace RHI
    {
        enum class UApiType
        {
#ifdef _WIN32
            D3D9,
            D3D11,
            D3D12,
#endif
            OpenGL
        };

#ifdef _WIN32
        inline bool IsD3DApi( UApiType nApiType )
        {
            return (nApiType == UApiType::D3D9 || nApiType == UApiType::D3D11 || nApiType == UApiType::D3D12);
        }
#endif
        inline bool IsOpenGLApi( UApiType nApiType )
        {
            return (nApiType == UApiType::OpenGL);
        }

        enum class UVertexFormat
            : uint32
        {
            FLOAT4,
            FLOAT3,
            FLOAT2,
            FLOAT,
            INT4,
            INT3,
            INT2,
            INT,
            USHORT4,
            USHORT2,
            UBYTE4,
            COLOR
        };

        enum class UPrimitiveType
        {
            Point,
            Line,
            LineStrip,
            Triangle,
            TriangleStrip
        };

        enum class UIndexFormat
        {
            U_Short,
            U_Int
        };

        enum UBufferUsageFlags
        {
            None = 0x0000,
            Static = 0x0001,
            Dynamic = 0x0002,
            AnyDynamic = (Dynamic),
        };



        /*
            D3DDECLUSAGE_POSITION = 0,
    D3DDECLUSAGE_BLENDWEIGHT,   // 1
    D3DDECLUSAGE_BLENDINDICES,  // 2
    D3DDECLUSAGE_NORMAL,        // 3
    D3DDECLUSAGE_PSIZE,         // 4
    D3DDECLUSAGE_TEXCOORD,      // 5
    D3DDECLUSAGE_TANGENT,       // 6
    D3DDECLUSAGE_BINORMAL,      // 7
    D3DDECLUSAGE_TESSFACTOR,    // 8
    D3DDECLUSAGE_POSITIONT,     // 9
    D3DDECLUSAGE_COLOR,         // 10
    D3DDECLUSAGE_FOG,           // 11
    D3DDECLUSAGE_DEPTH,         // 12
    D3DDECLUSAGE_SAMPLE,        // 13
        */
        enum class UVertexDeclareUsage
            : uint32
        {
            Position,
            Color,
            Normal,
            TexCoord,
            Sample
        };


        enum class UTextureFilterType : uint32
        {
            None = 0, // 只对Mipmap有效
            Point = 1,
            Linear = 2,
            Anisotropic = 3,
        };

        enum class UTextureAddress : uint32
        {
            Wrap = 1,
            Mirror = 2,
            Clamp = 3,
            Border = 4,
        };


        struct UTextureSamplerState
        {
        public:
            inline UTextureSamplerState()
                : nMipFilter( UTextureFilterType::Linear )
                , nMinFilter( UTextureFilterType::Linear )
                , nMagFilter( UTextureFilterType::Linear )
                , nAddressU( UTextureAddress::Wrap )
                , nAddressV( UTextureAddress::Wrap )
                , nAddressW( UTextureAddress::Wrap )
            {
            }

            bool operator==( const UTextureSamplerState& other ) const;
            bool operator!=( const UTextureSamplerState& other ) const;

            UTextureFilterType nMipFilter;// Mipmap
            UTextureFilterType nMinFilter;// Minification 缩小
            UTextureFilterType nMagFilter;// Magnification 放大

            UTextureAddress nAddressU;
            UTextureAddress nAddressV;
            UTextureAddress nAddressW;
        };

        enum class UShaderType
        {
            Vertex,
            Pixel,
        };

        enum class UShaderVariableType
        {
            VT_Unknown,
            VT_Boolean,
            VT_Int,
            VT_Float,
            VT_Vector4,
            VT_Matrix44,
            VT_Sampler2D
        };
    }
}



inline bool iberbar::RHI::UTextureSamplerState::operator==( const UTextureSamplerState& other ) const
{
    return (this->nMipFilter == other.nMipFilter &&
        this->nMinFilter == other.nMinFilter &&
        this->nMagFilter == other.nMagFilter &&
        this->nAddressU == other.nAddressU &&
        this->nAddressV == other.nAddressV &&
        this->nAddressW == other.nAddressW);
}


inline bool iberbar::RHI::UTextureSamplerState::operator!=( const UTextureSamplerState& other ) const
{
    return (this->nMipFilter != other.nMipFilter ||
        this->nMinFilter != other.nMinFilter ||
        this->nMagFilter != other.nMagFilter ||
        this->nAddressU != other.nAddressU ||
        this->nAddressV != other.nAddressV ||
        this->nAddressW != other.nAddressW);
}

