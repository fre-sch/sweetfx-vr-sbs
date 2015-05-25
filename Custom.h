//OCULUS DROID FOR NATIVE 3D SUCH AS ELITE DANGEROUS IN SBS MODE
#ifndef VignetteToggle
    #define VignetteToggle 1
#endif
#ifndef AspectRatioCorrection
    #define AspectRatioCorrection 1
#endif

// Warp using standard DK2 distortion parameters.
// Best left as-is but could be tweaked if required.
// DEFAULT = (1,0.22,0.24,0)
#ifndef HmdWarpParam
    #define HmdWarpParam float4(1.0, 0.22, 0.24, 0.0)
#endif

// Scale entire lens size smaller or larger.
// Larger numbers = smaller screen size.
// DEFAULT = 1.0
#define ScaleFactor float(1.0) 

//Seperation between lenses.
// X is first set of numbers, Y is second set (no need to offset Y).
// Negative X number = further apart.
// DEFAULT (0.0,0.0)
#define StereoOffset float2(0.0, 0.0)

// Chromatic Aberration.
// Change if your lenses requires correction.
// DK2 values = (0.99, -0.004, 1.014, 0)
// DEFAULT (None) = (1.0, 0.0, 1.0, 0)
#define ChromAbParam float4(1.0, 0.0, 1.0, 0)

// Distortion Scaling.
// Change if your lenses require different correction.
// A value of 0.55 = total coverage top to bottom but crops edges 
// 0.65 = better but with minor letterboxing. DEFAULT (0.65)
#define DistortionFactor float(0.65)

// Aspect ratio of each eye (for example 1920x1080 screen, each eye receives
// 960x1080. 960/1080 = 0.8888888888888889 (or 0.9 rounded up).
// This also affects distortion too. A value of 1.0 seems to work best for
// distortion without warping.
// DEFAULT = (1.0)
#define AspectRatio float(1.0)
//END CONFIGURATION

#define LensCenter float2(0.5f, 0.5f)
#define SBSOffset float2(1.0f, 0.0f)
#define SBSFactor float2(2.0f, 1.0f)

#define HmdWarp(theta) \
        theta * (HmdWarpParam.x \
                 + HmdWarpParam.y * rSq \
                 + HmdWarpParam.z * rSq * rSq \
                 + HmdWarpParam.w * rSq * rSq * rSq)


float4 OculusDroidWarp( float4 colorInput, float2 tex )
{
    float Width = 1f;
    float Height = 1f;
    float2 Scale;
    float2 ScaleIn;
    float2 scaleCenter = float2(0.5,0.5); 
    float3 tempColor;
    
    Scale.x = (Width / 2f) * DistortionFactor;
    Scale.y = (Height / 2f) * DistortionFactor * AspectRatio;
    
    ScaleIn.x = (2f / Width);
    ScaleIn.y = (2f / Height) / AspectRatio;

    if (tex.x < 0.5)
        tex = (tex * SBSFactor) - float2(0f, 0f) - StereoOffset;
    else
        tex = (tex *  SBSFactor) -  SBSOffset +  StereoOffset;
 
    float2 texCoord = tex;
    float2 theta = (texCoord - LensCenter) * ScaleIn; 
    float rSq =  theta.x * theta.x + theta.y * theta.y;
    float2 theta1 = HmdWarp(theta);
    texCoord = LensCenter + Scale * theta1; 
    
    if (tex.x < 0.5) {
        // Blue
        float2 thetaBlue = theta1 * (ChromAbParam.z + ChromAbParam.w * rSq);
        float2 tcBlue = LensCenter + Scale * thetaBlue;
#if AspectRatioCorrection == 1
        tcBlue.y = (tcBlue.y - scaleCenter) / 0.5 + scaleCenter;
#endif
        tcBlue.xy = (tcBlue.xy - scaleCenter) * ScaleFactor + scaleCenter;  
        tcBlue.x = tcBlue.x * 0.5;   
        float blue = myTex2D(s0, tcBlue).z;

        // Green (no scaling)
        float2 tcGreen = LensCenter + Scale * theta1;
#if AspectRatioCorrection == 1
        tcGreen.y = (tcGreen.y - scaleCenter) / 0.5 + scaleCenter;
#endif
        tcGreen.xy = (tcGreen.xy - scaleCenter) * ScaleFactor + scaleCenter;
        tcGreen.x = tcGreen.x * 0.5;     
        float green = myTex2D(s0, tcGreen).y;
       
        // Red
        float2 thetaRed = theta1 * (ChromAbParam.x + ChromAbParam.y * rSq);
        float2 tcRed = LensCenter + Scale * thetaRed;
#if AspectRatioCorrection == 1
        tcRed.y = (tcRed.y - scaleCenter) / 0.5 + scaleCenter;
#endif
        tcRed.xy = (tcRed.xy - scaleCenter) * ScaleFactor + scaleCenter;   
        tcRed.x = tcRed.x * 0.5;     
        float red = myTex2D(s0, tcRed).x;
    }
    else {
        // Blue
        float2 thetaBlue = theta1 * (ChromAbParam.z + ChromAbParam.w * rSq);
        float2 tcBlue = LensCenter + Scale * thetaBlue;
#if AspectRatioCorrection == 1
        tcBlue.y = (tcBlue.y - scaleCenter) / 0.5 + scaleCenter;
#endif
        tcBlue.xy = (tcBlue.xy - scaleCenter) * ScaleFactor + scaleCenter; 
        tcBlue.x = (tcBlue.x + 1.0) * 0.5;   
        float blue = myTex2D(s0, tcBlue).z;

        // Green (no scaling)
        float2 tcGreen = LensCenter + Scale * theta1;
#if AspectRatioCorrection == 1
        tcGreen.y = (tcGreen.y - scaleCenter) / 0.5 + scaleCenter;
#endif
        tcGreen.xy = (tcGreen.xy - scaleCenter) * ScaleFactor + scaleCenter;
        tcGreen.x = (tcGreen.x + 1.0) * 0.5;     
        float green = myTex2D(s0, tcGreen).y;

        // Red
        float2 thetaRed = theta1 * (ChromAbParam.x + ChromAbParam.y * rSq);
        float2 tcRed = LensCenter + Scale * thetaRed;
#if AspectRatioCorrection == 1
        tcRed.y = (tcRed.y - scaleCenter) / 0.5 + scaleCenter;
#endif
        tcRed.xy = (tcRed.xy - scaleCenter) * ScaleFactor + scaleCenter;  
        tcRed.x = (tcRed.x + 1.0) * 0.5;      
        float red = myTex2D(s0, tcRed).x;
    }

     //Scale rest of screen 
#if AspectRatioCorrection == 1
    tex.y = (tex.y - scaleCenter) / 0.5 + scaleCenter;
    texCoord.y = (texCoord.y - scaleCenter) / 0.5 + scaleCenter;
#endif  
    tex.xy = (tex.xy - scaleCenter) * ScaleFactor + scaleCenter;
    texCoord.xy = (texCoord.xy - scaleCenter) * ScaleFactor + scaleCenter;   

#if VignetteToggle == 1
    tempColor = float3(red, green, blue);

    if(texCoord.x>0.98){
      float vignetteFactor = 50-50*texCoord.x;
      tempColor*=vignetteFactor;
    }
    if(texCoord.y>0.98){
      float vignetteFactor = 50-50*texCoord.y;
      tempColor*=vignetteFactor;
    }
    if(texCoord.x<0.02){
      float vignetteFactor = 50*texCoord.x;
      tempColor*=vignetteFactor;
    }
    if(texCoord.y<0.02){
      float vignetteFactor = 50*texCoord.y;
      tempColor*=vignetteFactor;
    }
#endif

    if ((texCoord.y > 1.0f) || (texCoord.y < 0.0f)) { 
        return 0;
    }
    if ((texCoord.x > 1.0f) || (texCoord.x < 0.0f)) { 
        return 0;
    }
    else {
        return float4(tempColor, 1.0);
    }
}


float3 CustomPass(float4 position : SV_Position, float2 texcoord : TEXCOORD0) : SV_Target
{
    float4 color = myTex2D(s0, texcoord);
    color = OculusDroidWarp(color, texcoord);
    return color.rgb;
}
