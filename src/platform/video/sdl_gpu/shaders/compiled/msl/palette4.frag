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
    uint _80;
    do
    {
        uint2 _49 = uint2(uIndexTexture.get_width(), uIndexTexture.get_height());
        uint _51 = _49.y;
        uint _52 = _49.x * 2u;
        int2 _63 = clamp(int2(in.in_var_TEXCOORD0 * float2(float(_52), float(_51))), int2(0), int2(int(_52 - 1u), int(_51 - 1u)));
        int _64 = _63.x;
        uint4 _70 = uIndexTexture.read(uint2(int3(_64 / 2, _63.y, 0).xy), 0);
        uint _71 = _70.x;
        if ((_64 & 1) == 0)
        {
            _80 = _71 & 15u;
            break;
        }
        else
        {
            _80 = (_71 >> 4u) & 15u;
            break;
        }
        break; // unreachable workaround
    } while(false);
    float4 _88 = uPaletteTexture.sample(uPaletteSampler, float2((float(_80) + 0.5) * 0.0625, 0.5));
    float4 _90 = _88.zyxw * in.in_var_TEXCOORD1;
    if (_90.w == 0.0)
    {
        discard_fragment();
    }
    out.out_var_SV_Target0 = _90;
    return out;
}

