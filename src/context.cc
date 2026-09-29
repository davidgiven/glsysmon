#include "context.h"

Context::Context(App& a, ImGuiIf& i, Preferences& p, Timer& t):
    app(a),
    imgui(i),
    preferences(p),
    timer(t)
{
}
