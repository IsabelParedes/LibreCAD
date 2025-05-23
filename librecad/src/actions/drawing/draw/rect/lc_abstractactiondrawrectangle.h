/****************************************************************************
**
* Abstract base class for actions that draws a rectangle

Copyright (C) 2024 LibreCAD.org
Copyright (C) 2024 sand1024

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**********************************************************************/

#ifndef LC_ABSTRACTACTIONDRAWRECTANGLE_H
#define LC_ABSTRACTACTIONDRAWRECTANGLE_H

#include "lc_abstractactionwithpreview.h"

class LC_AbstractActionDrawRectangle:public LC_AbstractActionWithPreview {
    Q_OBJECT
public:
    /**
* defines modes for drawing rect corners
*/
    enum {
        CORNER_STRAIGHT, // plain rect
        CORNER_RADIUS,  // rounded corners
        CORNER_BEVEL // bevels
    };

    LC_AbstractActionDrawRectangle(const char *name, LC_ActionContext *actionContext, RS2::ActionType actionType = RS2::ActionNone);
    ~LC_AbstractActionDrawRectangle() override;

    bool isUsePolyline() const{return m_usePolyline;};
    void setUsePolyline(bool value){m_usePolyline = value;};
    void setRadius(double radius);
    double getRadius() const{return m_radius;};
    void setLengthX(double value);
    double getLengthX() const{return m_bevelX;};
    void setLengthY(double value);
    double getLengthY() const{return m_bevelY;};
    void setUcsAngleDegrees(double angle);
    double getUcsAngleDegrees() const;
    void setCornersMode(int value);
    int getCornersMode() const{return m_cornersDrawMode;};
    void setInsertionPointSnapMode(int value);
    int getInsertionPointSnapMode() const{return m_insertionPointSnapMode;};
    void setSnapToCornerArcCenter(bool b);
    bool isSnapToCornerArcCenter() const {return m_snapToCornerArcCenter;};
    void setEdgesDrawMode(int mode){m_edgesDrawMode = mode;};
    int getEdgesDrawMode() const{return m_edgesDrawMode;};
    void setBaseAngleFixed(bool val) {m_baseAngleIsFixed = val;};
    bool hasBaseAngle() const {return m_baseAngleIsFixed;}
protected:

    /**
     * Defines how edges should be drawn
     */
    enum {
        EDGES_BOTH, // all edges of rectangle are drawn
        EDGES_VERT, // only vertical edges of rectangle are drawn (2 parallel lines instead of rect)
        EDGES_HOR // only horizontal edges of rectangle are drawn
    };

    /**
      * Action States.
      */
    enum Status {
        SetPoint1,
        SetPoint2,
        SetPoint3,
        SetHeight,
        SetWidth,
        SetPoint1Snap,
        SetPoint2Snap,
        SetSize,
        SetAngle,
        SetCorners,
        SetBevels,
        SetRadius,
        SetEdges,
        SetInnerAngle,
        SetAngleFree,
        LAST_BASE_STATUS [[maybe_unused]]
    };

    /**
     * should resulting rect be polyline or not
     */
    bool m_usePolyline = false;
    /**
     * flag that controls how to draw corners
     */
    int m_cornersDrawMode =CORNER_STRAIGHT;
    /**
     * radius for rounded corners
     */
    double m_radius = 0.0;
    /**
     * x value of bevel
     */
    double m_bevelX = 0.0;
    /**
     * y value of bevel
     */
    double m_bevelY = 0.0;
    /**
     * angle of rectangle's rotation (angle between bottom edge and x axis)
     */
    double m_ucsBasisBaseAngleRad = 0;
    /**
     * flag that controls how to position rect relative to insertion point - may have different meanings in different actions
     */
    int m_insertionPointSnapMode = 0;
    /**
     * flag that indicates that snap should be performed taking into consideration rounded corners (if true, snap is not for, say, corner,
     * but to center of nearest rounded corner arc
     */
    bool m_snapToCornerArcCenter = false;

    /**
     * flag that indicates that angle for rect is specified and is fixed
     */
    bool m_baseAngleIsFixed = false;

    /**
     * mode that controls how edges of rect should be drawn
     */
    int m_edgesDrawMode = EDGES_BOTH;

    /**
     * Stores shape and snap for rectangle shape
     */
    struct ShapeData{
        RS_Polyline *resultingPolyline;
        RS_Vector snapPoint;
        RS_Vector centerPoint;
        double width;
        double height;
    };

    /*
     * shape data
     */
    ShapeData m_shapeData;

    void prepareCornersDrawMode(double &radiusX, double &radiusY, bool &drawComplex, bool &drawBulge) const;
    RS_Polyline* createPolylineByVertexes( RS_Vector bottomLeftCorner, RS_Vector bottomRightCorner,
                                           RS_Vector topRightCorner, RS_Vector topLeftCorner,
                                           bool drawBulge, bool drawComplex,
                                           double radiusX, double radiusY) const;

    virtual ShapeData createPolyline(const RS_Vector &snapPoint)  = 0;

    void createShapeData(const RS_Vector &snapPoint);
    virtual void processCommandValue(double value, bool &toMainStatus) = 0;
    virtual bool processCustomCommand(int status, const QString &command, bool &toMainStatus) = 0;
    virtual void doProcessCoordinateEvent(const RS_Vector &coord, bool isZero, int status);
    virtual void doUpdateMouseButtonHints(int status);
    virtual void doAddPolylineToListOfEntities(RS_Polyline *polyline, QList<RS_Entity *> &list, bool preview);
    static void normalizeCorners(RS_Vector &bottomLeftCorner, RS_Vector &bottomRightCorner, RS_Vector &topRightCorner, RS_Vector &topLeftCorner);
    bool doProcessCommand(int status, const QString &c) override;
    void doPreparePreviewEntities(LC_MouseEvent *e, RS_Vector &snap, QList<RS_Entity *> &list, int status) override;
    void stateUpdated(bool toMainStatus);
    double getActualBaseAngle() const;
    void doPrepareTriggerEntities(QList<RS_Entity *> &list) override;
    bool doCheckMayTrigger() override;
    void doAfterTrigger() override;
    RS_Vector doGetRelativeZeroAfterTrigger() override;
    void doBack(LC_MouseEvent *pEvent, int status) override;
    bool doCheckPolylineEntityAllowedInTrigger(int index) const;
    void onCoordinateEvent(int status, bool isZero, const RS_Vector &pos) override;
    void updateMouseButtonHints() override;
    void doSetAngle(double value);
};

#endif // LC_ABSTRACTACTIONDRAWRECTANGLE_H
