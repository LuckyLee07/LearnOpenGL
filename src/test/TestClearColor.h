#ifndef __TEST_CLEAR_COLOR_H__
#define __TEST_CLEAR_COLOR_H__

#include "Test.h"

namespace test
{
    class TestClearColor : public Test
    {
    public:
        TestClearColor();
        virtual ~TestClearColor();

        void OnUpdate(float deltatime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        float m_ClearColor[4];
    };
}

#endif