Texture2D uTexture : register(t0, space2);
SamplerState uSampler : register(s0, space2);

struct PSInput {
    float2 texCoord : TEXCOORD0;
    float4 color : TEXCOORD1;
};

float4 main(PSInput input) : SV_Target0 {
    float4 color = uTexture.Sample(uSampler, input.texCoord) * input.color;
    return float4(color.rgb, 1.0);
}
