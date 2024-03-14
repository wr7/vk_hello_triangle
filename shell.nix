{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
  nativeBuildInputs = with pkgs.buildPackages; [
    meson
    ninja
    shaderc
    pkg-config
    gcc
    clang-tools # Can be removed but is necessary for clangd
    vulkan-loader
    vulkan-validation-layers
    vulkan-utility-libraries
    vulkan-headers
    glfw
    gdb
  ];
}
