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

fragment main0_out main0(main0_in in [[stage_in]], texture2d<uint> uIndexTexture [[texture(0)]], texture2d<float> uPaletteTexture [[texture(1)]], sampler uPaletteSampler [[sampler(1)]])
{
    main0_out out = {};
    uint2 _41 = uint2(uIndexTexture.get_width(), uIndexTexture.get_height());
    uint _42 = _41.x;
    uint _43 = _41.y;
    float4 _68 = uPaletteTexture.sample(uPaletteSampler, float2((float(uIndexTexture.read(uint2(int3(clamp(int2(in.in_var_TEXCOORD0 * float2(float(_42), float(_43))), int2(0), int2(int(_42 - 1u), int(_43 - 1u))), 0).xy), 0).x) + 0.5) * 0.00390625, 0.5));
    float4 _70 = _68.zyxw * in.in_var_TEXCOORD1;
    if (_70.w == 0.0)
    {
        discard_fragment();
    }
    out.out_var_SV_Target0 = _70;
    return out;
}

