{ pkgs ? import <nixpkgs> {} }:
 {
  qpidEnv = pkgs.stdenvNoCC.mkDerivation {
    name = "dev-shell";
    nativeBuildInputs = with pkgs.buildPackages; [
      # runtime dependencies
      glfw
      # build dependencies
      gcc13
      meson
      ninja
      shaderc
      pkg-config
      vulkan-loader
      vulkan-validation-layers
      vulkan-utility-libraries
      vulkan-headers
      ffmpeg-headless
      # for debugging/development
      clang-tools
      gdb
      renderdoc
    ];
  };
}
