struct PSInput {
    float2 texCoord : TEXCOORD0;
    float4 color : TEXCOORD1;
};

float4 main(PSInput input) : SV_Target0 {
    return input.color;
}
