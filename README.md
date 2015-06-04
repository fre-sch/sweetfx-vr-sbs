# sweetfx-vr-sbs
Reshade+SweetFX (http://reshade.me) shader emulating Oculus Rift barrel distortion and chromatic abberation. Specifically created to work with Elite Dangerous Side-by-Side 3D.

Copy Custom.h to Sweetfx/Shaders/Custom.h.
Add the following at the end of SweetFx/SweetFX_settings.txt:

       /*-----------------------------------------------------------.
      /                       Custom settings                       /
      '-----------------------------------------------------------*/
    #define VignetteToggle        1  // [0 or 1]: fade image edges to black
    #define AspectRatioCorrection 0  // [0 or 1]: Correct stretching of SBS imagery
    #define HmdWarpParam          float4(1.0, 0.22, 0.24, 0.0)  // Adjust the barrel distortion
    #define ChromAbParam          float4(1.0, 0.0, 1.0, 0)  // Chromatic Aberration, change if your lenses requires correction.

I'm using these settings:

    #define VignetteToggle 1
    #define VignetteFactor 3f
    #define AspectRatioCorrection 0
    #define ChromaticAberration 0
    #define HmdWarpParam float4(1.40, 0.08, 0.08, 0)
