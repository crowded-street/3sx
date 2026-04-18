Texture2D<uint> uIndexTexture : register(t0, space2);
Texture2D<float4> uPaletteTexture : register(t1, space2);
SamplerState uPaletteSampler : register(s1, space2);

struct PSInput {
    float2 texCoord : TEXCOORD0;
    float4 color : TEXCOORD1;
};

uint get_index(float2 uv) {
    uint width;
    uint height;
    uIndexTexture.GetDimensions(width, height);

    int2 pixel = int2(uv * float2(width, height));
    pixel = clamp(pixel, int2(0, 0), int2(width - 1, height - 1));

    return uIndexTexture.Load(int3(pixel, 0)).r;
}

float4 main(PSInput input) : SV_Target0 {
    uint index = get_index(input.texCoord);
    float paletteU = (float(index) + 0.5) / 256.0;
    float4 color = uPaletteTexture.Sample(uPaletteSampler, float2(paletteU, 0.5));
    color = color.bgra;
    color *= input.color;

    if (color.a == 0.0) {
        discard;
    }

    return color;
}
