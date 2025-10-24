#ifndef FL7_FONTS_RENDER_TESTRENDERER_H
#define FL7_FONTS_RENDER_TESTRENDERER_H
#include "./AbstractRenderer.h"



namespace fl7::fonts::render {



class TestRenderer
    : public AbstractRenderer
{

public:
    ~TestRenderer() noexcept override = default;



private:
    void _emit_glyph(const Glyph& glyph, const State& state) override;

}; // class TestRenderer



} // namespace fl7::fonts::render

#endif // FL7_FONTS_RENDER_TESTRENDERER_H
