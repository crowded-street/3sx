#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct main0_out
{
    float4 out_var_SV_Target0 [[color(0)]];
};

struct main0_in
{
    float2 in_var_TEXCOORD0 [[user(locn0)]];
    float4 in_var_TEXCOORD1 [[user(locn1)]];
};

fragment main0_out main0(main0_in in [[stage_in]], texture2d<float> uTexture [[texture(0)]], sampler uSampler [[sampler(0)]])
{
    main0_out out = {};
    float4 _28 = uTexture.sample(uSampler, in.in_var_TEXCOORD0);
    float4 _30 = _28.zyxw * in.in_var_TEXCOORD1;
    if (_30.w == 0.0)
    {
        discard_fragment();
    }
    out.out_var_SV_Target0 = _30;
    return out;
}

