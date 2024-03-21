{ pkgs ? import <nixpkgs> {} }:
 {
  qpidEnv = pkgs.stdenvNoCC.mkDerivation {
    name = "dev-shell";
    nativeBuildInputs = with pkgs.buildPackages; [
      gcc13
      meson
      ninja
      shaderc
      pkg-config
      clang-tools # Can be removed but is necessary for clangd
      vulkan-loader
      vulkan-validation-layers
      vulkan-utility-libraries
      vulkan-headers
      glfw
      gdb
    ];
  };
}
