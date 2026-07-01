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
    float2 _72 = in.in_var_TEXCOORD0 * float2(384.0, 224.0);
    float2 _74 = floor(_72) + float2(0.5);
    float2 _75 = _72 - _74;
    float2 _80 = (_74 + (((_75 * 4.0) * _75) * _75)) * float2(0.00260416674427688121795654296875, 0.0044642859138548374176025390625);
    _80.x = mix(_80.x, in.in_var_TEXCOORD0.x, 0.300000011920928955078125);
    float _86 = _75.y;
    float _87 = _86 * _86;
    float _88 = _87 * _87;
    bool _93 = fract(in.in_var_TEXCOORD0.y * (-0.5)) < 0.5;
    float _95 = 0.150000035762786865234375;
    float _96 = 1.0;
    float4 _102 = powr(tex0.sample(texSampler, _80), float4(2.400000095367431640625));
    float4 _118 = (_102 * mix(1.0, mix((1.4500000476837158203125 - (6.0 * (_87 - (2.0499999523162841796875 * _88)))) * (_93 ? 0.150000035762786865234375 : 1.0), 1.0 - (6.0 * (_88 - ((2.7999999523162841796875 * _88) * _87))), dot(_102.xyz, float3(0.266640007495880126953125))), fast::clamp(CRTUniforms.scanline_intensity, 0.0, 1.0))) * float4(1.15999996662139892578125, 1.08000004291534423828125, 1.15999996662139892578125, 1.0);
    float3 _119 = _118.xyz;
    float3 _123 = fast::clamp(mix(float3(dot(_119, float3(0.2989999949932098388671875, 0.58700001239776611328125, 0.114000000059604644775390625))), _119, float3(1.12000000476837158203125)), float3(0.0), float3(1.0));
    out.out_var_SV_Target = fast::clamp(powr(float4(_123.x, _123.y, _123.z, _118.w), float4(0.4545454680919647216796875)), float4(0.0), float4(1.0));
    return out;
}

