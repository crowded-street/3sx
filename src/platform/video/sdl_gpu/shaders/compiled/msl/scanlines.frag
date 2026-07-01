#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct type_CRTUniforms
{
    float scanline_intensity;
    packed_float3 padding;
};

struct main0_out
{
    float4 out_var_SV_Target [[color(0)]];
};

struct main0_in
{
    float2 in_var_TEXCOORD0 [[user(locn0)]];
};

fragment main0_out main0(main0_in in [[stage_in]], constant type_CRTUniforms& CRTUniforms [[buffer(0)]], texture2d<float> tex0 [[texture(0)]], sampler texSampler [[sampler(0)]])
{
    main0_out out = {};
    float _71 = fast::clamp(CRTUniforms.scanline_intensity, 0.0, 1.0);
    float2 _76 = in.in_var_TEXCOORD0 * float2(384.0, 224.0);
    float2 _78 = floor(_76) + float2(0.5);
    float2 _79 = _76 - _78;
    float2 _84 = (_78 + (((_79 * 4.0) * _79) * _79)) * float2(0.00260416674427688121795654296875, 0.0044642859138548374176025390625);
    _84.x = mix(_84.x, in.in_var_TEXCOORD0.x, 0.300000011920928955078125);
    float _90 = _79.y;
    float _91 = _90 * _90;
    float _92 = _91 * _91;
    bool _97 = fract(in.in_var_TEXCOORD0.y * (-0.5)) < 0.5;
    float _99 = 0.150000035762786865234375;
    float _100 = 1.0;
    float4 _106 = powr(tex0.sample(texSampler, _84), float4(2.400000095367431640625));
    float4 _122 = (_106 * mix(1.0, mix((1.4500000476837158203125 - (6.0 * (_91 - (2.0499999523162841796875 * _92)))) * (_97 ? 0.150000035762786865234375 : 1.0), 1.0 - (6.0 * (_92 - ((2.7999999523162841796875 * _92) * _91))), dot(_106.xyz, float3(0.266640007495880126953125))), _71)) * float4(1.15999996662139892578125, 1.08000004291534423828125, 1.15999996662139892578125, 1.0);
    float3 _123 = _122.xyz;
    float3 _127 = fast::clamp(mix(float3(dot(_123, float3(0.2989999949932098388671875, 0.58700001239776611328125, 0.114000000059604644775390625))), _123, float3(1.12000000476837158203125)), float3(0.0), float3(1.0));
    out.out_var_SV_Target = mix(tex0.sample(texSampler, in.in_var_TEXCOORD0), fast::clamp(powr(float4(_127.x, _127.y, _127.z, _122.w), float4(0.4545454680919647216796875)), float4(0.0), float4(1.0)), float4(_71));
    return out;
}

