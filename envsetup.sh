rm -rf ./build
meson setup build

alias b='meson compile -C build'
alias r='b && build/vulkanhello'
alias re='b && renderdoccmd capture build/vulkanhello'
