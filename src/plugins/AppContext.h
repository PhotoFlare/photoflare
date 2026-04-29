/*
    This file is part of Photoflare.

    Photoflare is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Photoflare is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Photoflare.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef APP_CONTEXT_H
#define APP_CONTEXT_H

#include <QImage>
#include <QString>
#include <QSettings>
#include <QWidget>
#include <QAction>

/**
 * AppContext
 *
 * The stable API surface exposed to all plugins via initialise(AppContext*).
 * Implemented by MainWindow. Plugins must not hold references to any other
 * internal class.
 *
 * PhotoFlare-specific notes
 * ─────────────────────────
 * currentImage() / markCanvasDirty():
 *   These exist for rare cases where a plugin needs direct canvas access
 *   (e.g. a live-preview tool outside the standard filter dialog). For normal
 *   filter plugins, the host calls apply() itself and writes the result back
 *   via applyFilteredImage(), so these methods are not needed in apply().
 *
 * pushUndoState():
 *   PaintWidget::setImage() automatically pushes an undo snapshot, so calling
 *   pushUndoState() before a filter apply() is informational — the host stores
 *   the label for UI purposes. Direct canvas writers must call it once BEFORE
 *   modifying *currentImage(), then call markCanvasDirty() afterwards.
 */
class AppContext {
public:
    virtual ~AppContext() = default;

    // ── Canvas access ────────────────────────────────────────────────────────

    /// Returns a pointer to the currently open image, or nullptr if none.
    /// The pointer is stable for the duration of a single UI operation.
    /// Use pushUndoState() before modifying, markCanvasDirty() afterwards.
    virtual QImage* currentImage() = 0;

    /// Commit any direct modifications to *currentImage() back to the canvas
    /// and trigger a repaint.
    virtual void markCanvasDirty() = 0;

    // ── Undo ─────────────────────────────────────────────────────────────────

    /// Snapshot the current image into the undo stack with a human-readable
    /// label. Call this once before a destructive direct edit.
    /// Not required when using IFilterPlugin::apply() — the host handles it.
    virtual void pushUndoState(const QString& actionLabel) = 0;

    // ── UI registration ──────────────────────────────────────────────────────

    /// Add an action to a menu. menuPath uses "/" as separator.
    /// "Filters/Noise" → Filters → Noise submenu.
    /// The host takes ownership of the QAction.
    virtual void registerMenuAction(const QString& menuPath, QAction* action) = 0;

    /// Register a widget as a dockable panel. The host manages docking.
    virtual void registerDockPanel(const QString& title, QWidget* panel) = 0;

    /// Show a transient message in the status bar.
    virtual void showStatusMessage(const QString& message, int timeoutMs = 3000) = 0;

    // ── Settings ─────────────────────────────────────────────────────────────

    /// QSettings scoped to the calling plugin's ID.
    virtual QSettings& pluginSettings(const QString& pluginId) = 0;

    // ── Utilities ────────────────────────────────────────────────────────────

    /// Main window pointer — use only to parent dialogs.
    virtual QWidget* mainWindow() = 0;

    /// Application version string, e.g. "2.1.0".
    virtual QString appVersion() const = 0;
};

#endif // APP_CONTEXT_H
