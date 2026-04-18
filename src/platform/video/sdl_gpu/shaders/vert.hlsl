struct VSInput {
    float3 position : TEXCOORD0;
    float2 texCoord : TEXCOORD1;
    float4 color : TEXCOORD2;
};

struct VSOutput {
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD0;
    float4 color : TEXCOORD1;
};

VSOutput main(VSInput input) {
    VSOutput output;
    output.position = float4(input.position, 1.0);
    output.texCoord = input.texCoord;
    output.color = input.color;
    return output;
}
