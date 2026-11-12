#pragma once

#include "include/texture.h"
#include "include/canvas.h"


namespace engine
{
    class Animation
    {
        public:
        Animation(const Texture* sheet, Vec2i frameSize, int count, float fps, bool loop = true);

        void update(float dt);
        void reset();
        Recti frame() const;
        bool finished() const;
        const Texture* texture() const;

        private:
        const Texture* m_sheet;
        Vec2i m_frameSize;
        int m_count, m_index = 0;
        float m_fps, m_time = 0.0f;
        bool m_loop;
    };
    struct Sprite
    {
        Animation* anim = nullptr;
        Vec2i pos;
        int scale = 1;
        bool flipX = false;
        float opacity = 1.0f;

        void draw(Canvas& c) const
        {
            if (!anim || !anim->texture()) return;
            Recti f = anim->frame();
            c.drawTexturePartPx(pos, *anim->texture(), f,
                                {(flipX ? -1 : 1) * f.w * scale, f.h * scale},opacity);
        }

    };

}