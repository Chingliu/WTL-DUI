
#ifndef __desktop_window_manager_h__
#define __desktop_window_manager_h__

#pragma once

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "ui/views/widget/widget.h"
#include "window_manager.h"

namespace gfx
{
    class Point;
}

namespace views
{

    namespace desktop
    {
        class WindowController;

        // A tentative window manager for views destktop until we have *right*
        // implementation based on aura/layer API. This is minimum
        // implmenetation and complicated actio like moving transformed window
        // doesn't work.
        class DesktopWindowManager : public views::WindowManager,
            public Widget::Observer
        {
        public:
            DesktopWindowManager(Widget* desktop);
            virtual ~DesktopWindowManager();

            void UpdateWindowsAfterScreenSizeChanged(const gfx::Rect& new_size);

            // view::WindowManager implementations:
            virtual void StartMoveDrag(views::Widget* widget,
                const gfx::Point& point);
            virtual void StartResizeDrag(views::Widget* widget,
                const gfx::Point& point,
                int hittest_code);
            virtual bool SetMouseCapture(views::Widget* widget);
            virtual bool ReleaseMouseCapture(views::Widget* widget);
            virtual bool HasMouseCapture(const views::Widget* widget) const;
            virtual bool HandleKeyEvent(views::Widget* widget,
                const views::KeyEvent& event);
            virtual bool HandleMouseEvent(views::Widget* widget,
                const views::MouseEvent& event);

            virtual void Register(Widget* widget);

        private:
            // Overridden from Widget::Observer.
            virtual void OnWidgetClosing(Widget* widget);
            virtual void OnWidgetVisibilityChanged(Widget* widget, bool visible);
            virtual void OnWidgetActivationChanged(Widget* widget, bool active);

            void SetMouseCapture();
            void ReleaseMouseCapture();
            bool HasMouseCapture() const;

            void Activate(Widget* widget);

            // Returns true if a deactivated widget at the location was activated. Returns
            // false otherwise.
            bool ActivateWidgetAtLocation(Widget* widget, const gfx::Point& point);

            views::Widget* desktop_;
            views::Widget* mouse_capture_;
            views::Widget* active_widget_;

            // An unordered list of all the top-level Widgets.
            std::vector<views::Widget*> toplevels_;

            scoped_ptr<WindowController> window_controller_;

            DISALLOW_COPY_AND_ASSIGN(DesktopWindowManager);
        };

        // An behavioral interface for objects implements window resize/movement.
        class WindowController
        {
        public:
            WindowController();
            virtual ~WindowController();
            virtual bool OnMouseEvent(const views::MouseEvent& event) = 0;

        private:
            DISALLOW_COPY_AND_ASSIGN(WindowController);
        };

    } //namespace desktop
} //namespace view

#endif //__desktop_window_manager_h__