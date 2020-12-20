

texture g_texture;
bool g_useTexture;


// 绑定通道0中的纹理 
sampler g_textureSampler : register(s0);

struct PixelInputType
{
    float4 pos : POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD0;
};
float4 Main(PixelInputType input) : COLOR
{
    if (g_useTexture == true)
    {
        return tex2D(g_textureSampler, input.texcoord) * input.color;
    }
		
    return input.color;
}