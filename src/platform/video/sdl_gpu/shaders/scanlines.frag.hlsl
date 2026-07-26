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

Texture2D tex0 : register(t0, space2);
SamplerState texSampler : register(s0, space2);

cbuffer CRTUniforms : register(b0, space3)
{
    float scanline_intensity;
    float3 padding;
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

float3 AdjustSaturation(float3 color, float saturation)
{
    float intensity = dot(color, float3(0.299, 0.587, 0.114));

    float3 gray = intensity.xxx;

    return saturate(lerp(gray, color, saturation));
}

float4 main(VSOutput input) : SV_Target
{
    float scanline_strength = saturate(scanline_intensity);
    float4 source_color = tex0.Sample(texSampler, input.texcoord);

    float2 coords = input.texcoord * texture_size;

    float2 pixelCenter = floor(coords) + 0.5;

    float2 dx = coords - pixelCenter;

    float2 texcoord =
        (pixelCenter + 4.0 * dx * dx * dx) / texture_size;

    texcoord.x = lerp(texcoord.x, input.texcoord.x, BLURSCALEX);

    float Y = dx.y * dx.y;
    float YY = Y * Y;

    float whichmask = frac(input.texcoord.y * -0.5);

    float mask =
        1.0 + ((whichmask < 0.5) ? -MASK_DARK : 0.0);

    float4 color = GammaIn(tex0.Sample(texSampler, texcoord));

    float scanLineWeight =
        BRIGHTBOOST -
        LOWLUMSCAN * (Y - 2.05 * YY);

    float scanLineWeightB =
        1.0 -
        HILUMSCAN * (YY - 2.8 * YY * Y);

    float maskFade = 0.3333 * MASK_FADE;

    float brightness =
        dot(color.rgb, float3(maskFade, maskFade, maskFade));

    color *= lerp(
        1.0,
        lerp(
            scanLineWeight * mask,
            scanLineWeightB,
            brightness),
        scanline_strength);

    color *= float4(
        RED_GAIN * COLOR_BOOST,
        GREEN_GAIN * COLOR_BOOST,
        BLUE_GAIN * COLOR_BOOST,
        COLOR_BOOST);

    color.rgb = AdjustSaturation(color.rgb, SATURATION);

    color = saturate(GammaOut(color));

    return lerp(source_color, color, scanline_strength);
}
