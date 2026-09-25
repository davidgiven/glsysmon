CXX      ?= g++
CC       ?= gcc
CXXFLAGS ?= -std=c++20 -Wall -Wextra -O2 -g

BUILD := .obj
GEN   := $(BUILD)/wayland
BIN   := glsysmon

PKG_CONFIG ?= pkg-config

SDL_CFLAGS     := $(shell $(PKG_CONFIG) --cflags sdl3)
SDL_LIBS       := $(shell $(PKG_CONFIG) --libs sdl3)
IMGUI_DIR      := dep/imgui
IMGUI_CFLAGS   := -I$(IMGUI_DIR)
IMPLOT_DIR     := dep/implot
IMPLOT_CFLAGS  := -I$(IMPLOT_DIR)
IMHTML_DIR      := dep/imhtml
IMHTML_CFLAGS   := -I$(IMHTML_DIR)
LITEHTML_DIR      := dep/litehtml
LITEHTML_CFLAGS := -I$(LITEHTML_DIR)/include -I$(LITEHTML_DIR)/include/litehtml -I$(LITEHTML_DIR)/src -I$(LITEHTML_DIR)/src/gumbo/include
LITEHTML_LIBS   :=
GUMBO_CFLAGS    :=
GUMBO_LIBS      :=
X11_CFLAGS     := $(shell $(PKG_CONFIG) --cflags x11)
X11_LIBS       := $(shell $(PKG_CONFIG) --libs x11)
WAYLAND_CFLAGS := $(shell $(PKG_CONFIG) --cflags wayland-client)
WAYLAND_LIBS   := $(shell $(PKG_CONFIG) --libs wayland-client)
TOMLPLUSPLUS_CFLAGS := $(shell $(PKG_CONFIG) --cflags tomlplusplus)
TOMLPLUSPLUS_LIBS   := $(shell $(PKG_CONFIG) --libs tomlplusplus)
STB_CFLAGS          := $(shell $(PKG_CONFIG) --cflags stb 2>/dev/null)
STB_LIBS            := $(shell $(PKG_CONFIG) --libs stb 2>/dev/null)

WAYLAND_SCANNER := $(shell $(PKG_CONFIG) --variable=wayland_scanner wayland-scanner)

IMGUI_BACKENDS_DIR := $(IMGUI_DIR)/backends
FONT_TTF               := $(IMGUI_DIR)/misc/fonts/DroidSans.ttf
FONT_TOOL              := $(BUILD)/binary_to_compressed_c
FONT_GEN_CPP           := $(BUILD)/DroidSansFont.cpp
FONT_GEN_OBJ           := $(BUILD)/DroidSansFont.o

WAYLAND_XML               := third_party/wayland/wlr-layer-shell-unstable-v1.xml
WAYLAND_PROTOCOL_HEADER   := $(GEN)/wlr-layer-shell-client-protocol.h
WAYLAND_PROTOCOL_CODE     := $(GEN)/wlr-layer-shell-client-protocol.c

#xdg - shell is referenced by the layer - shell get_popup request; \
    Debian ships the
#XML in wayland - protocols.Only its interface tables(for xdg_popup) are needed.
XDG_SHELL_XML               := $(firstword $(wildcard /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml) $(wildcard /usr/share/qt6/wayland/protocols/xdg-shell/xdg-shell.xml))
XDG_SHELL_PROTOCOL_HEADER   := $(GEN)/xdg-shell-client-protocol.h
XDG_SHELL_PROTOCOL_CODE     := $(GEN)/xdg-shell-client-protocol.c

COMMON_CFLAGS := $(CXXFLAGS) -I$(CURDIR)/src -I$(BUILD) $(SDL_CFLAGS) $(IMGUI_CFLAGS) $(IMPLOT_CFLAGS) $(IMHTML_CFLAGS) $(LITEHTML_CFLAGS) $(X11_CFLAGS) \
                  $(WAYLAND_CFLAGS) $(TOMLPLUSPLUS_CFLAGS) -MMD -MP
LITEHTML_SRCS := $(wildcard $(LITEHTML_DIR)/src/*.cpp)
LITEHTML_OBJS := $(patsubst $(LITEHTML_DIR)/src/%.cpp,$(BUILD)/litehtml/%.o,$(LITEHTML_SRCS))
GUMBO_SRCS := $(wildcard $(LITEHTML_DIR)/src/gumbo/*.c)
GUMBO_OBJS := $(patsubst $(LITEHTML_DIR)/src/gumbo/%.c,$(BUILD)/gumbo/%.o,$(GUMBO_SRCS))

SRC_OBJS := \
	$(BUILD)/configuration.o \
	$(BUILD)/display/dock.o \
	$(BUILD)/display/fallback_dock_impl.o \
	$(BUILD)/display/imgui_frame_renderer_impl.o \
	$(BUILD)/display/wayland_dock_impl.o \
	$(BUILD)/display/x11_dock_impl.o \
	$(BUILD)/imgui_app_impl.o \
	$(BUILD)/imgui_ui_impl.o \
	$(BUILD)/main.o \
	$(BUILD)/preferences/cli_preferences_impl.o \
	$(BUILD)/preferences/combined_preferences_impl.o \
	$(BUILD)/preferences/default_preferences_impl.o \
	$(BUILD)/preferences/map_preferences_impl.o \
	$(BUILD)/preferences/preferences.o \
	$(BUILD)/preferences/string_value.o \
	$(BUILD)/preferences/toml_preferences_impl.o \
	$(BUILD)/sensors/clock_sensor_impl.o \
	$(BUILD)/sensors/cpu_sensor_impl.o \
	$(BUILD)/sensors/hostname_sensor_impl.o \
	$(BUILD)/sensors/network_sensor_impl.o \
	$(BUILD)/sensors/sensor.o \
	$(BUILD)/sensors/sensors.o \
	$(BUILD)/sensors/temperature_sensor_impl.o \
	$(BUILD)/timer.o \
	$(BUILD)/views/clock_view_impl.o \
	$(BUILD)/views/cpu_view_impl.o \
	$(BUILD)/views/hostname_view_impl.o \
	$(BUILD)/views/network_view_impl.o \
	$(BUILD)/views/style.o \
	$(BUILD)/views/temperature_view_impl.o \
	$(BUILD)/views/view.o \
	$(BUILD)/views/views.o \
	$(FONT_GEN_OBJ)

BACKEND_OBJS := \
	$(BUILD)/imgui_impl_sdl3.o \
	$(BUILD)/imgui_impl_sdlgpu3.o

IMGUI_OBJS := \
	$(BUILD)/imgui.o \
	$(BUILD)/imgui_draw.o \
	$(BUILD)/imgui_tables.o \
	$(BUILD)/imgui_widgets.o

IMPLOT_OBJS := \
	$(BUILD)/implot.o \
	$(BUILD)/implot_items.o

IMHTML_OBJS := \
	$(BUILD)/imhtml.o

WAYLAND_OBJS := \
	$(GEN)/wlr-layer-shell-client-protocol.o \
	$(GEN)/xdg-shell-client-protocol.o

OBJS := $(SRC_OBJS) $(IMGUI_OBJS) $(IMPLOT_OBJS) $(IMHTML_OBJS) $(LITEHTML_OBJS) $(GUMBO_OBJS) $(BACKEND_OBJS) $(WAYLAND_OBJS)

TEST_BUILD  := $(BUILD)/tests
TEST_CFLAGS := $(COMMON_CFLAGS) $(STB_CFLAGS) -I$(CURDIR)/src

TEST_UNIT   := $(TEST_BUILD)/unit_tests
TEST_TIMER  := $(TEST_BUILD)/timer_tests
TEST_GRAPH_MIXIN := $(TEST_BUILD)/graph_mixin_test
TEST_PREFERENCES := $(TEST_BUILD)/preferences_test
TEST_RENDER_HOSTNAME := $(TEST_BUILD)/render_fake_hostname
TEST_RENDER_CLOCK    := $(TEST_BUILD)/render_fake_clock
TEST_RENDER_CPU      := $(TEST_BUILD)/render_fake_cpu
TEST_RENDER_TEMPERATURE := $(TEST_BUILD)/render_fake_temperature
TEST_RENDER := $(TEST_RENDER_HOSTNAME) $(TEST_RENDER_CLOCK) $(TEST_RENDER_CPU) $(TEST_RENDER_TEMPERATURE)

# Objects needed by every test binary: the modules the app's components pull in.
TEST_OBJS := \
	$(BUILD)/configuration.o \
	$(BUILD)/imgui_ui_impl.o \
	$(BUILD)/display/imgui_frame_renderer_impl.o \
	$(BUILD)/preferences/preferences.o \
	$(BUILD)/preferences/string_value.o \
	$(BUILD)/preferences/cli_preferences_impl.o \
	$(BUILD)/preferences/toml_preferences_impl.o \
	$(BUILD)/preferences/combined_preferences_impl.o \
	$(BUILD)/preferences/default_preferences_impl.o \
	$(BUILD)/preferences/map_preferences_impl.o \
	$(BUILD)/sensors/sensors.o \
	$(BUILD)/views/style.o \
	$(BUILD)/timer.o \
	$(BUILD)/views/view.o \
	$(BUILD)/views/views.o \
	$(BUILD)/views/clock_view_impl.o \
	$(BUILD)/views/cpu_view_impl.o \
	$(BUILD)/views/hostname_view_impl.o \
	$(BUILD)/views/network_view_impl.o \
	$(BUILD)/views/temperature_view_impl.o \
	$(BUILD)/sensors/clock_sensor_impl.o \
	$(BUILD)/sensors/cpu_sensor_impl.o \
	$(BUILD)/sensors/hostname_sensor_impl.o \
	$(BUILD)/sensors/network_sensor_impl.o \
	$(BUILD)/sensors/sensor.o \
	$(BUILD)/sensors/temperature_sensor_impl.o \
	$(IMGUI_OBJS) $(IMPLOT_OBJS) $(IMHTML_OBJS) $(LITEHTML_OBJS) $(GUMBO_OBJS) $(BACKEND_OBJS) $(FONT_GEN_OBJ)

DEPS := $(OBJS:.o=.d) $(TEST_BUILD)/unit_tests.d $(TEST_BUILD)/timer_tests.d $(TEST_BUILD)/graph_mixin_test.d $(TEST_BUILD)/render_frame.d \
         $(TEST_BUILD)/render_lib.d $(TEST_BUILD)/render_fake_hostname.d \
         $(TEST_BUILD)/render_fake_clock.d $(TEST_BUILD)/render_fake_cpu.d \
         $(BUILD)/imhtml.d \
         $(FONT_GEN_CPP:.cpp=.d)

TEST_RENDER_COMMON_OBJS := $(TEST_BUILD)/render_frame.o \
	$(TEST_BUILD)/render_lib.o

all: $(BIN)

$(BIN): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(SDL_LIBS) $(X11_LIBS) $(WAYLAND_LIBS) \
		$(TOMLPLUSPLUS_LIBS) $(LITEHTML_LIBS)

$(FONT_TOOL): $(IMGUI_DIR)/misc/fonts/binary_to_compressed_c.cpp
	@mkdir -p $(dir $@)
	$(CXX) -O2 -o $@ $<

$(FONT_GEN_CPP): $(FONT_TTF) $(FONT_TOOL)
	@mkdir -p $(dir $@)
	$(FONT_TOOL) -nostatic $< DroidSansFont > $@
	sed -i 's/^const unsigned/extern const unsigned/' $@

$(FONT_GEN_OBJ): $(FONT_GEN_CPP)
	@mkdir -p $(dir $@)
	$(CXX) $(COMMON_CFLAGS) -c -o $@ $<

$(GEN)/wlr-layer-shell-client-protocol.h: $(WAYLAND_XML)
	@mkdir -p $(GEN)
	$(WAYLAND_SCANNER) client-header < $< > $@

$(GEN)/wlr-layer-shell-client-protocol.c: $(WAYLAND_XML)
	@mkdir -p $(GEN)
	$(WAYLAND_SCANNER) private-code < $< > $@

$(GEN)/wlr-layer-shell-client-protocol.o: $(GEN)/wlr-layer-shell-client-protocol.c
	$(CC) -O2 $(WAYLAND_CFLAGS) -MMD -MP -c -o $@ $<

$(GEN)/xdg-shell-client-protocol.h: $(XDG_SHELL_XML)
	@mkdir -p $(GEN)
	$(WAYLAND_SCANNER) client-header < $< > $@

$(GEN)/xdg-shell-client-protocol.c: $(XDG_SHELL_XML)
	@mkdir -p $(GEN)
	$(WAYLAND_SCANNER) private-code < $< > $@

$(GEN)/xdg-shell-client-protocol.o: $(GEN)/xdg-shell-client-protocol.c
	$(CC) -O2 $(WAYLAND_CFLAGS) -MMD -MP -c -o $@ $<

$(BUILD)/%.o: src/%.cc $(WAYLAND_PROTOCOL_HEADER)
	@mkdir -p $(dir $@)
	$(CXX) $(COMMON_CFLAGS) -c -o $@ $<

$(BUILD)/imgui.o: $(IMGUI_DIR)/imgui.cpp
	@mkdir -p $(BUILD)
	$(CXX) $(COMMON_CFLAGS) -c -o $@ $<

$(BUILD)/imgui_draw.o: $(IMGUI_DIR)/imgui_draw.cpp
	@mkdir -p $(BUILD)
	$(CXX) $(COMMON_CFLAGS) -c -o $@ $<

$(BUILD)/imgui_tables.o: $(IMGUI_DIR)/imgui_tables.cpp
	@mkdir -p $(BUILD)
	$(CXX) $(COMMON_CFLAGS) -c -o $@ $<

$(BUILD)/imgui_widgets.o: $(IMGUI_DIR)/imgui_widgets.cpp
	@mkdir -p $(BUILD)
	$(CXX) $(COMMON_CFLAGS) -c -o $@ $<

$(BUILD)/implot.o: $(IMPLOT_DIR)/implot.cpp
	@mkdir -p $(BUILD)
	$(CXX) $(COMMON_CFLAGS) -c -o $@ $<

$(BUILD)/implot_items.o: $(IMPLOT_DIR)/implot_items.cpp
	@mkdir -p $(BUILD)
	$(CXX) $(COMMON_CFLAGS) -c -o $@ $<

$(BUILD)/imhtml.o: $(IMHTML_DIR)/imhtml.cpp
	@mkdir -p $(BUILD)
	$(CXX) $(COMMON_CFLAGS) -c -o $@ $<

$(BUILD)/litehtml/%.o: $(LITEHTML_DIR)/src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(COMMON_CFLAGS) -c -o $@ $<

$(BUILD)/gumbo/%.o: $(LITEHTML_DIR)/src/gumbo/%.c
	@mkdir -p $(dir $@)
	$(CC) -O2 -g -Wall -Wextra -I$(LITEHTML_DIR)/src/gumbo/include -I$(LITEHTML_DIR)/src/gumbo/include/gumbo -MMD -MP -c -o $@ $<

$(BUILD)/imgui_impl_sdl3.o: $(IMGUI_BACKENDS_DIR)/imgui_impl_sdl3.cpp
	@mkdir -p $(BUILD)
	$(CXX) $(COMMON_CFLAGS) -c -o $@ $<

$(BUILD)/imgui_impl_sdlgpu3.o: $(IMGUI_BACKENDS_DIR)/imgui_impl_sdlgpu3.cpp
	@mkdir -p $(BUILD)
	$(CXX) $(COMMON_CFLAGS) -c -o $@ $<

$(TEST_BUILD)/%.o: tests/%.cc
	@mkdir -p $(TEST_BUILD)
	$(CXX) $(TEST_CFLAGS) -c -o $@ $<

$(TEST_UNIT): $(TEST_BUILD)/unit_tests.o $(TEST_OBJS)
	$(CXX) -o $@ $^ $(SDL_LIBS) $(TOMLPLUSPLUS_LIBS) $(LITEHTML_LIBS)

$(TEST_TIMER): $(TEST_BUILD)/timer_tests.o $(BUILD)/timer.o
	$(CXX) -o $@ $^

$(TEST_GRAPH_MIXIN): $(TEST_BUILD)/graph_mixin_test.o $(TEST_OBJS)
	$(CXX) -o $@ $^ $(SDL_LIBS) $(TOMLPLUSPLUS_LIBS) $(LITEHTML_LIBS)

$(TEST_PREFERENCES): $(TEST_BUILD)/preferences_test.o $(TEST_OBJS)
	$(CXX) -o $@ $^ $(SDL_LIBS) $(TOMLPLUSPLUS_LIBS) $(LITEHTML_LIBS)

$(TEST_RENDER_HOSTNAME): $(TEST_BUILD)/render_fake_hostname.o \
	$(TEST_RENDER_COMMON_OBJS) $(TEST_OBJS)
	$(CXX) -o $@ $^ $(SDL_LIBS) $(TOMLPLUSPLUS_LIBS) $(LITEHTML_LIBS) \
		$(STB_LIBS)

$(TEST_RENDER_CLOCK): $(TEST_BUILD)/render_fake_clock.o \
	$(TEST_RENDER_COMMON_OBJS) $(TEST_OBJS)
	$(CXX) -o $@ $^ $(SDL_LIBS) $(TOMLPLUSPLUS_LIBS) $(LITEHTML_LIBS) \
		$(STB_LIBS)

$(TEST_RENDER_CPU): $(TEST_BUILD)/render_fake_cpu.o \
	$(TEST_RENDER_COMMON_OBJS) $(TEST_OBJS)
	$(CXX) -o $@ $^ $(SDL_LIBS) $(TOMLPLUSPLUS_LIBS) $(LITEHTML_LIBS) \
		$(STB_LIBS)

$(TEST_RENDER_TEMPERATURE): $(TEST_BUILD)/render_fake_temperature.o \
	$(TEST_RENDER_COMMON_OBJS) $(TEST_OBJS)
	$(CXX) -o $@ $^ $(SDL_LIBS) $(TOMLPLUSPLUS_LIBS) $(LITEHTML_LIBS) \
		$(STB_LIBS)

run: $(BIN)
	./$(BIN)

test: $(TEST_UNIT) $(TEST_TIMER) $(TEST_GRAPH_MIXIN) $(TEST_PREFERENCES) $(TEST_RENDER)
	./$(TEST_UNIT)
	./$(TEST_TIMER)
	./$(TEST_GRAPH_MIXIN)
	./$(TEST_PREFERENCES)
	./$(TEST_RENDER_HOSTNAME)
	./$(TEST_RENDER_CLOCK)
	./$(TEST_RENDER_CPU)
	./$(TEST_RENDER_TEMPERATURE)

# Compilation database for clangd; every src/**/*.cc and tests/*.cc builds with
# their respective flags.
SRC_CC := $(wildcard src/*.cc) $(wildcard src/preferences/*.cc) \
           $(wildcard src/display/*.cc) $(wildcard src/views/*.cc) \
           $(wildcard src/sensors/*.cc)
TEST_CC := $(wildcard tests/*.cc)
compile_commands.json: $(SRC_CC) $(TEST_CC)
	@mkdir -p $(BUILD)
	@{ printf '[\n'; first=1; for f in $(sort $(SRC_CC) $(TEST_CC)); do \
		if [ $$first -eq 1 ]; then first=0; else printf ',\n'; fi; \
		obj_dir="$(CURDIR)/$(BUILD)"; \
		if [ "$$(dirname "$$f")" = "tests" ]; then obj_dir="$$obj_dir/tests"; \
		elif [ "$$(dirname "$$f")" != "src" ]; then obj_dir="$$obj_dir/$$(basename $$(dirname "$$f"))"; fi; \
		cmd="$(CXX) $(COMMON_CFLAGS) $(STB_CFLAGS) -I$(CURDIR)/src -c -o $$obj_dir/$$(basename $$f .cc).o $$f"; \
		printf '  {"directory": "$(CURDIR)", "file": "$(CURDIR)/%s", "command": "%s"}' "$$f" "$$cmd"; \
	done; printf '\n]\n'; } > $@

clean:
	rm -rf $(BUILD) $(BIN) compile_commands.json

-include $(DEPS)

.PHONY: all run test clean
