#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct main0_out
{
    float2 out_var_TEXCOORD0 [[user(locn0)]];
    float2 out_var_TEXCOORD1 [[user(locn1)]];
    float4 gl_Position [[position]];
};

struct main0_in
{
    float4 in_var_POSITION [[attribute(0)]];
    float2 in_var_TEXCOORD0 [[attribute(1)]];
};

vertex main0_out main0(main0_in in [[stage_in]])
{
    main0_out out = {};
    out.gl_Position = in.in_var_POSITION;
    out.out_var_TEXCOORD0 = in.in_var_TEXCOORD0;
    out.out_var_TEXCOORD1 = float2(0.00260416674427688121795654296875, 0.0044642859138548374176025390625);
    return out;
}

