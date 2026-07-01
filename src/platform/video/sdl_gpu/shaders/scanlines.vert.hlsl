#define SPOT_WIDTH      0.9
#define SPOT_HEIGHT     0.65
#define COLOR_BOOST     1.0

#define InputGamma      2.4
#define OutputGamma     2.2

#define BLURSCALEX      0.30
#define LOWLUMSCAN      6.0
#define HILUMSCAN       6.0
#define BRIGHTBOOST     1.45

#define MASK_DARK       0.85
#define MASK_FADE       0.8

#define RED_GAIN        1.16
#define GREEN_GAIN      1.08
#define BLUE_GAIN       1.16

#define SATURATION      1.12

static const float2 texture_size = float2(384.0, 224.0);

struct VSInput
{
    float4 position : POSITION;
    float2 texcoord : TEXCOORD0;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float2 one      : TEXCOORD1;
};

float4 GammaIn(float4 c)
{
    return pow(c, InputGamma);
}

float4 GammaOut(float4 c)
{
    return pow(c, 1.0 / OutputGamma);
}

VSOutput main(VSInput input)
{
    VSOutput OUT;

    OUT.position = input.position;
    OUT.texcoord = input.texcoord;

    OUT.one = float2(
        1.0 / texture_size.x,
        1.0 / texture_size.y);

    return OUT;
}
