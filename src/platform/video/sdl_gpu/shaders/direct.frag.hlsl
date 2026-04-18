Texture2D uTexture : register(t0, space2);
SamplerState uSampler : register(s0, space2);

struct PSInput {
    float2 texCoord : TEXCOORD0;
    float4 color : TEXCOORD1;
};

float4 main(PSInput input) : SV_Target0 {
    float4 raw_color = uTexture.Sample(uSampler, input.texCoord);
    // SDL GPU doesn't support BGRA texture formats, so we swizzle manually here
    float4 color = raw_color.bgra * input.color;

    if (color.a == 0.0) {
        discard;
    }

    return color;
}
