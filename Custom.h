//OCULUS DROID FOR NATIVE 3D SUCH AS ELITE DANGEROUS IN SBS MODE

//CONFIGURATION
// A soft vignette at the edge of the lense correction.
// Toggle vignette.
// DEFAULT = 1
#ifndef VignetteToggle
#define VignetteToggle 1
#endif
// Strength of vignette [1f to 6f]
// DEFAULT = 2f
#ifndef VignetteFactor
#define VignetteFactor 3f
#endif

// Toggle aspect ratio correction for SBS stereo.
// DEFAULT = 1
#ifndef AspectRatioCorrection
#define AspectRatioCorrection 1
#endif

// Chromatic Aberration.
#ifndef ChromaticAberration
#define ChromaticAberration 0
#endif

// Change if your lenses requires correction.
// DK2 = (0.99, -0.004, 1.014, 0).
// DEFAULT (no aberration) = (1.0, 0.0, 1.0, 0)
#ifndef ChromAbParam
#define ChromAbParam float4(1.0, 0.0, 1.0, 0)
#endif

// Warp using standard DK2 distortion parameters.
// Best left as-is but could be tweaked if required.
// DEFAULT = (1,0.22,0.24,0)
#ifndef HmdWarpParam
#define HmdWarpParam float4(1, 0.22, 0.24, 0)
#endif

// Scale entire lens size smaller or larger.
// Larger numbers = smaller screen size.
// DEFAULT = 1.0
#define SCALE_FACTOR float(1.0)

// Separation between lenses.
// X is first set of numbers, Y is second set (no need to offset Y).
// Negative X number = further apart.
// DEFAULT (0.0,0.0)
#define STEREO_OFFSET float2(0.0, 0.0)

// Distortion Scaling.
// Change if your lenses require different correction.
// A value of 0.55 = total coverage top to bottom but crops edges
// 0.65 = better but with minor letterboxing.
// DEFAULT = (0.65)
#define DISTORTION_FACTOR float(0.65)

// Aspect ratio of each eye.
// For example 1920x1080 screen, each eye recieves 960x1080.
// 960/1080 = 0.8889 (or 0.9 rounded up). This also affects distortion too.
// A value of 1.0 seems to work best for distortion without warping.
// DEFAULT = (1.0)
#define ASPECT_RATIO float(1.0)
//END CONFIGURATION


#define LENS_CENTER float2(0.5f, 0.5f)
#define SBS_FACTOR float2(2, 1)
#define SCALE_CENTER float2(0.5, 0.5)
#define WIDTH (1f)
#define HEIGHT (1f)


float4 applyWarpChromAb(float2 scale, float2 scaleIn, float rSq, float2 theta1, float offset) {
    // Blue
    float2 thetaBlue = theta1 * (ChromAbParam.z + ChromAbParam.w * rSq);
    float2 tcBlue = LENS_CENTER + scale * thetaBlue;
#if AspectRatioCorrection == 1
    tcBlue.y = (tcBlue.y - SCALE_CENTER) / 0.5 + SCALE_CENTER;
#endif
    tcBlue.xy = (tcBlue.xy - SCALE_CENTER) * SCALE_FACTOR + SCALE_CENTER;
    tcBlue.x = (tcBlue.x + offset) * 0.5;
    
    // Green (no scaling)
    float2 tcGreen = LENS_CENTER + scale * theta1;
#if AspectRatioCorrection == 1
    tcGreen.y = (tcGreen.y - SCALE_CENTER) / 0.5 + SCALE_CENTER;
#endif
    tcGreen.xy = (tcGreen.xy - SCALE_CENTER) * SCALE_FACTOR + SCALE_CENTER;
    tcGreen.x = (tcGreen.x + offset) * 0.5;
    
    // Red
    float2 thetaRed = theta1 * (ChromAbParam.x + ChromAbParam.y * rSq);
    float2 tcRed = LENS_CENTER + scale * thetaRed;
#if AspectRatioCorrection == 1
    tcRed.y = (tcRed.y - SCALE_CENTER) / 0.5 + SCALE_CENTER;
#endif
    tcRed.xy = (tcRed.xy - SCALE_CENTER) * SCALE_FACTOR + SCALE_CENTER;
    tcRed.x = (tcRed.x + offset) * 0.5;
    
    return float4(
        tex2D(s0, tcRed).x,
        tex2D(s0, tcGreen).y,
        tex2D(s0, tcBlue).z,
        1
    );
}

float4 applyWarp(float2 scale, float2 scaleIn, float rSq, float2 theta1, float offset) {
    float2 tc = LENS_CENTER + scale * theta1;
#if AspectRatioCorrection == 1
    tc = (tc - SCALE_CENTER) / 0.5 + SCALE_CENTER;
#endif
    tc = (tc - SCALE_CENTER) * SCALE_FACTOR + SCALE_CENTER;
    tc.x = (tc.x + offset) * 0.5;
    
    return float4(tex2D(s0, tc).rgb, 1f);
}

float4 NativeSBSWarp(float2 tex) {
    float4 resultColor = float4(0, 0, 0, 1);
    float2 scale = float2(
        (WIDTH / 2.0) * DISTORTION_FACTOR,
        (HEIGHT / 2.0) * DISTORTION_FACTOR * ASPECT_RATIO
    );
    float2 scaleIn = float2(
        (2.0 / WIDTH),
        (2.0 / HEIGHT) / ASPECT_RATIO
    );
    float2 texCoord;
    float2 xoffset;

    if (tex.x < 0.5) {
        xoffset = float2(0f, 0f);
        tex = (tex * SBS_FACTOR) - xoffset - STEREO_OFFSET;
    }
    else {
        xoffset = float2(1f, 0f);
        tex = (tex * SBS_FACTOR) - xoffset + STEREO_OFFSET;
    }

    texCoord = tex;
    float2 theta = (texCoord - LENS_CENTER) * scaleIn;
    float rSq =  theta.x * theta.x + theta.y * theta.y;
    float2 theta1 = theta * (
        HmdWarpParam.x
        + HmdWarpParam.y * rSq
        + HmdWarpParam.z * rSq * rSq
        + HmdWarpParam.w * rSq * rSq * rSq
    );
    texCoord = LENS_CENTER + scale * theta1;
#if ChromaticAberration == 1
    resultColor = applyWarpChromAb(scale, scaleIn, rSq, theta1, xoffset.x);
#else
    resultColor = applyWarp(scale, scaleIn, rSq, theta1, xoffset.x);
#endif

    //scale rest of screen
#if AspectRatioCorrection == 1
    tex.y = (tex.y - SCALE_CENTER) / 0.5 + SCALE_CENTER;
    texCoord.y = (texCoord.y - SCALE_CENTER) / 0.5 + SCALE_CENTER;
#endif
    tex.xy = (tex.xy - SCALE_CENTER) * SCALE_FACTOR + SCALE_CENTER;
    texCoord.xy = (texCoord.xy - SCALE_CENTER) * SCALE_FACTOR + SCALE_CENTER;

#if VignetteToggle == 1
    texCoord = -texCoord * texCoord + texCoord;
    resultColor *= saturate(texCoord.x * texCoord.y * 100f * VignetteFactor);
#endif

    if ((texCoord.y > 1.0f) || (texCoord.y < 0.0f)
        || (texCoord.x > 1.0f) || (texCoord.x < 0.0f)) {
        return 0;
    }
    return resultColor;
}


float3 CustomPass(float4 position : SV_Position, float2 texCoord : TEXCOORD0) : SV_Target {
    return NativeSBSWarp(texCoord).rgb;
}
