# sweetfx-vr-sbs
Reshade+SweetFX shader emulating Oculus Rift barrel distortion and chromatic abberation. Specifically created to work with Elite Dangerous Side-by-Side 3D.

Copy Custom.h to Sweetfx/Shaders/Custom.h.
Add the following at the end of SweetFx/SweetFX_settings.txt:

       /*-----------------------------------------------------------.
      /                       Custom settings                       /
      '-----------------------------------------------------------*/
    #define VignetteToggle        1  // [0 or 1]: fade image edges to black
    #define AspectRatioCorrection 0  // [0 or 1]: Correct stretching when running in 16:9 resolutions
    #define HmdWarpParam          float4(1.0, 0.22, 0.24, 0.0)  // Adjust the barrel distortion
