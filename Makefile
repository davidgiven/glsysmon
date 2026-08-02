CXX      ?= g++
CC       ?= gcc
CXXFLAGS ?= -std=c++20 -Wall -Wextra -O2 -g

BUILD := build
GEN   := $(BUILD)/wayland
BIN   := glrellm

PKG_CONFIG ?= pkg-config

SDL_CFLAGS     := $(shell $(PKG_CONFIG) --cflags sdl3)
SDL_LIBS       := $(shell $(PKG_CONFIG) --libs sdl3)
IMGUI_CFLAGS   := $(shell $(PKG_CONFIG) --cflags imgui)
IMGUI_LIBS     := $(shell $(PKG_CONFIG) --libs imgui)
X11_CFLAGS     := $(shell $(PKG_CONFIG) --cflags x11)
X11_LIBS       := $(shell $(PKG_CONFIG) --libs x11)
WAYLAND_CFLAGS := $(shell $(PKG_CONFIG) --cflags wayland-client)
WAYLAND_LIBS   := $(shell $(PKG_CONFIG) --libs wayland-client)
TOMLPLUSPLUS_CFLAGS := $(shell $(PKG_CONFIG) --cflags tomlplusplus)
TOMLPLUSPLUS_LIBS   := $(shell $(PKG_CONFIG) --libs tomlplusplus)

# Fruit ships no .pc file; headers are in the default include path.
FRUIT_LIBS := -lfruit

WAYLAND_SCANNER := $(shell $(PKG_CONFIG) --variable=wayland_scanner wayland-scanner)

# Dear ImGui platform/render backends shipped by the libimgui-dev package.
IMGUI_BACKENDS_DIR     ?= /usr/share/doc/libimgui-dev/examples/backends
IMGUI_BACKEND_CFLAGS   := -I/usr/include/imgui -I/usr/include/imgui/backends

WAYLAND_XML               := third_party/wayland/wlr-layer-shell-unstable-v1.xml
WAYLAND_PROTOCOL_HEADER   := $(GEN)/wlr-layer-shell-client-protocol.h
WAYLAND_PROTOCOL_CODE     := $(GEN)/wlr-layer-shell-client-protocol.c

# xdg-shell is referenced by the layer-shell get_popup request; Debian ships the
# XML in wayland-protocols. Only its interface tables (for xdg_popup) are needed.
XDG_SHELL_XML               := /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml
XDG_SHELL_PROTOCOL_HEADER   := $(GEN)/xdg-shell-client-protocol.h
XDG_SHELL_PROTOCOL_CODE     := $(GEN)/xdg-shell-client-protocol.c

COMMON_CFLAGS := $(CXXFLAGS) $(SDL_CFLAGS) $(IMGUI_CFLAGS) $(X11_CFLAGS) \
                 $(WAYLAND_CFLAGS) $(TOMLPLUSPLUS_CFLAGS) -I$(BUILD)

SRC_OBJS := \
	$(BUILD)/cli_preferences_impl.o \
	$(BUILD)/combined_preferences_impl.o \
	$(BUILD)/dock.o \
	$(BUILD)/fallback_dock_impl.o \
	$(BUILD)/imgui_app_impl.o \
	$(BUILD)/imgui_ui_impl.o \
	$(BUILD)/main.o \
	$(BUILD)/toml_preferences_impl.o \
	$(BUILD)/wayland_dock_impl.o \
	$(BUILD)/x11_dock_impl.o

BACKEND_OBJS := \
	$(BUILD)/imgui_impl_sdl3.o \
	$(BUILD)/imgui_impl_sdlgpu3.o

WAYLAND_OBJS := \
	$(GEN)/wlr-layer-shell-client-protocol.o \
	$(GEN)/xdg-shell-client-protocol.o

OBJS := $(SRC_OBJS) $(BACKEND_OBJS) $(WAYLAND_OBJS)

all: $(BIN)

$(BIN): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(SDL_LIBS) $(IMGUI_LIBS) $(X11_LIBS) $(WAYLAND_LIBS) \
		$(TOMLPLUSPLUS_LIBS) $(FRUIT_LIBS)

$(GEN)/wlr-layer-shell-client-protocol.h: $(WAYLAND_XML)
	@mkdir -p $(GEN)
	$(WAYLAND_SCANNER) client-header < $< > $@

$(GEN)/wlr-layer-shell-client-protocol.c: $(WAYLAND_XML)
	@mkdir -p $(GEN)
	$(WAYLAND_SCANNER) private-code < $< > $@

$(GEN)/wlr-layer-shell-client-protocol.o: $(GEN)/wlr-layer-shell-client-protocol.c
	$(CC) -O2 $(WAYLAND_CFLAGS) -c -o $@ $<

$(GEN)/xdg-shell-client-protocol.h: $(XDG_SHELL_XML)
	@mkdir -p $(GEN)
	$(WAYLAND_SCANNER) client-header < $< > $@

$(GEN)/xdg-shell-client-protocol.c: $(XDG_SHELL_XML)
	@mkdir -p $(GEN)
	$(WAYLAND_SCANNER) private-code < $< > $@

$(GEN)/xdg-shell-client-protocol.o: $(GEN)/xdg-shell-client-protocol.c
	$(CC) -O2 $(WAYLAND_CFLAGS) -c -o $@ $<

$(BUILD)/%.o: src/%.cc $(WAYLAND_PROTOCOL_HEADER)
	@mkdir -p $(BUILD)
	$(CXX) $(COMMON_CFLAGS) -c -o $@ $<

$(BUILD)/imgui_impl_sdl3.o: $(IMGUI_BACKENDS_DIR)/imgui_impl_sdl3.cpp
	@mkdir -p $(BUILD)
	$(CXX) $(COMMON_CFLAGS) $(IMGUI_BACKEND_CFLAGS) -c -o $@ $<

$(BUILD)/imgui_impl_sdlgpu3.o: $(IMGUI_BACKENDS_DIR)/imgui_impl_sdlgpu3.cpp
	@mkdir -p $(BUILD)
	$(CXX) $(COMMON_CFLAGS) $(IMGUI_BACKEND_CFLAGS) -c -o $@ $<

run: $(BIN)
	./$(BIN)

# Compilation database for clangd; every src/*.cc builds with COMMON_CFLAGS.
compile_commands.json: $(wildcard src/*.cc)
	@mkdir -p $(BUILD)
	@{ printf '[\n'; first=1; for f in $(sort $(wildcard src/*.cc)); do \
		if [ $$first -eq 1 ]; then first=0; else printf ',\n'; fi; \
		cmd="$(CXX) $(COMMON_CFLAGS) -c -o $(CURDIR)/$(BUILD)/$$(basename $$f .cc).o $$f"; \
		printf '  {"directory": "$(CURDIR)", "file": "$(CURDIR)/%s", "command": "%s"}' "$$f" "$$cmd"; \
	done; printf '\n]\n'; } > $@

clean:
	rm -rf $(BUILD) $(BIN) compile_commands.json

.PHONY: all run clean
