#ifndef __GameState_H__
#define __GameState_H__

#include "Blokaroka.h"
#include <map>

#define BLOK_WIDTH 15
#define BLOK_HEIGHT 18

#define BLOK_MATE_LENGTH 15

#define COLOR_WHITE         RGB(242,243,242)
#define COLOR_RED           RGB(196,40,27)
#define COLOR_YELLOW        RGB(245,205,47)
#define COLOR_GREEN         RGB(75,151,74)
#define COLOR_BLUE          RGB(13,105,171)

#define COLOR_AMETHYST_0    RGB(254,233,243)
#define COLOR_AMETHYST_1    RGB(238,166,201)
#define COLOR_AMETHYST_2    RGB(214,114,151)
#define COLOR_AMETHYST_3    RGB(170,90,127)

#define COLOR_TOPAZ_0       RGB(190,229,227)
#define COLOR_TOPAZ_1       RGB(118,206,215)
#define COLOR_TOPAZ_2       RGB(60,186,218)
#define COLOR_TOPAZ_3       RGB(45,133,189)

#define COLOR_PERIDOT_0     RGB(199,251,155)
#define COLOR_PERIDOT_1     RGB(145,217,85)
#define COLOR_PERIDOT_2     RGB(102,178,49)
#define COLOR_PERIDOT_3     RGB(65,131,28)


class GameState
{
public:
    enum 
    {
        GS_Static,
        GS_Drag_Free,
        GS_Drag_Mating,
        GS_Drag_Splitting
    };

    class Blok 
    {
    public:
        Blok(COLORREF color, int x, int y) :
            m_wColor(color),
            m_iX(x),
            m_iY(y),
            m_iRenderX(x),
            m_iRenderY(y),
            m_iMark(0),
            m_poLeft(NULL),
            m_poRight(NULL),
            m_poTop(NULL),
            m_poBottom(NULL),
            m_fBottomWeightHeld(-1.0f),
            m_fTopWeightHeld(-1.0f),
            m_iDistance(-1),
            m_iRenderState(BRS_SOLID),
            m_fBlockWeight(0),
            m_poTopDesiredMate(NULL),
            m_poBottomDesiredMate(NULL),
            m_iGroup(0)
        { };

        // Blok Render states:
        enum
        {
            BRS_SOLID,
            BRS_GLOW,
            BRS_NO_ATTACH
        };

        int GetColorID(); // Return an index into the color pallate.
        void SetColorForMenu(int menu); // Based on ColorID set this to the pallate based on the menu passed.

        void ClearBlok(HDC hdc); // Designed to clear the area this Blok occupies for redrawing, WIP: not used.
        void DrawBlok(HDC hdc); // Renders this Blok at its current location color and state.

        bool CursorIntersectsPoint(int x, int y); // Test x,y to see if it is inside the Blok including the trim.
        bool IntersectsPoint(int x, int y); // Text x,y to see if it is inside the face of the Blok, no trim.
        bool IntersectsBlok(Blok* poOther); // Do AABB box test between this and poOther.
        bool IntersectsAnyBlok(); // Recursivley test if this structure intersects with any other Blok.

        bool CanMateWithBlok(Blok* poOther); // Tests the size of the gap between this Blok and poOther to see if a connection can be made.
        Blok* FindMate(); // Recursively iterates over this structure to look for any potential prick that this Blok can connect to.
        void Mate(); // Connect this Blok with the stored m_poTopDesiredMate or m_poBottomDesiredMate

        bool CanDetachUp(); // Recursivly iterates over this structure to see if detaching the Bloks above will free a new sub structure.
        void DetachUp(); // Detach all the top Bloks connected to this Blok.

        bool CanDetachDown(); // Recursivly iterate over this structure to see if detaching the Bloks below will free a new sub structure.
        void DetachDown(); // Detach all the bottom Bloks connected to this Blok.

        void SetBlokPosition(int x, int y); // Set the actual position of the Blok, used in physics computations.
        void SetBlokRenderPosition(int x, int y); // Set the display position of the Blok, this is where we see the Blok and is adjusted when Bloks are lined up to connect.

        void AttachLeft(Blok * poLeft, bool bMoveOther = true); // Add a new left Blok, if bMoveOther the other Blok is positioned correctly.
        void AttachRight(Blok * poRight, bool bMoveOther = true); // Add a new right Blok, if bMoveOther the other Blok is positioned correctly.
        void AttachTop(Blok * poTop, bool bMoveOther = true); // Add a new top Blok, if bMoveOther the other Blok is positioned correctly.
        void AttachBottom(Blok * poBottom, bool bMoveOther = true); // Add a new bottom Blok, if bMoveOther the other Blok is positioned correctly.

        void CalculateDistances(); // UNUSED, physics WIP for randomly detaching Bloks based on weight.
        void CalculateWeights(); // UNUSED, physics WIP for randomly detaching Bloks based on weight.
        void BreakStressedConnections(); // UNUSED, physics WIP for randomly detaching Bloks based on weight.
        
        void SetMarks(int mark); // Recursively set the mark of all neighbors.
        void SetRenderState(int iNewState); // Recursivly set the render state.

        int ComputeDepth(); // Compute the render depth used for the z buffer.

        void RecursiveSetGroup(int group); // Used for render depth calculations, WIP.

        // ----- Basic Getter Functions ------------------------------------------------------------------ //
        int GetX() const { return m_iX; };
        int GetY() const { return m_iY; };
        COLORREF GetColor() const { return m_wColor; };
        const Blok* GetLeft() const { return m_poLeft; };
        const Blok* GetRight() const { return m_poRight; };
        const Blok* GetTop() const { return m_poTop; };
        const Blok* GetBottom() const { return m_poBottom; };
        int GetRenderState() { return m_iRenderState; };
        int GetGroup() { return m_iGroup; };

        // ----- Basic Setter Functions ------------------------------------------------------------------ //
        void SetGroup(int group) { m_iGroup = group; };

        // ----- Basic Has Functions ------------------------------------------------------------------ //
        bool HasLeft() const { return m_poLeft != NULL; };
        bool HasRight() const { return m_poRight != NULL; };
        bool HasTop() const { return m_poTop != NULL; };
        bool HasBottom() const { return m_poBottom != NULL; };

    private:



        void RecursiveFixNeighbors(int x, int y);
        bool RecursiveIntersectsAnyBlok();
        void ClearMates();
        GameState::Blok* RecursiveFindMate();
        void RecursiveMakeMatches();
        void RecursiveMate();
        void RecursiveFixRenderNeighbors(int x, int y);

        bool RecursiveCanDetachBlokUp(int mark);
        bool RecursiveCanDetachBlokDown(int mark);

        void RecursiveClearHoldWeights();

        void RecursiveClearDistances(); // Used for WIP physics code.
        void RecursiveCalculateDistances(int distance=0); // Used for WIP physics code.
        float GetWeight(); // Used for WIP physics code;

        int GetNumConnections(Blok* poOther);
        void RecursiveBreakStressedConnections(float fThreshold);

        void RecursiveSetRenderState(int iNewState);


        // ----- Blok Data Members ------------------------------------------------------------------ //
        int m_iX, m_iY; // Real position used for physics calculations.
        int m_iRenderX, m_iRenderY; // Display position used to line up Bloks to show potential connections.

        int m_iMark; // This is used for recursion in various ways, usually it stores if this Blok has been visited.

        int m_iRenderState; // This can be set to make the Bloks glow or transparent.
        int m_iGroup; // Used for Blok depth calculations - WIP.

        COLORREF m_wColor; // Render color.

        int m_iDistance; // Used for WIP physics code.
        float m_fTopWeightHeld; // Used for WIP physics code.
        float m_fBottomWeightHeld; // Used for WIP physics code.
        float m_fBlockWeight; // Used for WIP physics code.

        // Neighbors, basically the Bloks are a graph data structure each Blok having 4 potential branches.
        Blok * m_poLeft;
        Blok * m_poRight;
        Blok * m_poTop;
        Blok * m_poBottom;

        Blok * m_poTopDesiredMate; // Used to store calculated potential connection Bloks.
        Blok * m_poBottomDesiredMate; // Used to store calculated potential connection Bloks.
    };

    GameState() :
        m_iAttach1(0),
        m_iLastX(0),
        m_iLastY(0),
        m_iState(0),
        m_iYOffset(0),
        m_iXOffset(0),
        m_poMovingMate(NULL),
        m_poStaticMate(NULL),
        m_poSelectedBlok(NULL),
        m_vpoBloks(NULL)
    {

    }

    void GenerateBloks();
    void Shutdown();
    void Update();

    void SetState(int iNewState);
    int GetState();

    void SetDragging(Blok* poDragged, int x, int y);

    void MoveSelected(int x, int y);

    Blok* GetBlokAt(int x, int y);

    void PlayAttach();
    void PlayDetach();

    void DumpGamestate(std::wstring cstrName);
    bool ReadGamestate(std::wstring cstrName);
    void SetSplitDragging(GameState::Blok* poDragged, int x, int y);

    void SetColorsForMenu(int menu);

    int m_iXOffset, m_iYOffset;

    std::vector<Blok*> m_vpoBloks;

    Blok * m_poMovingMate;
    Blok * m_poStaticMate;

private:

    Blok * m_poSelectedBlok;
    int m_iState;

    int m_iLastX;
    int m_iLastY;

    int m_iAttach1;
};

extern GameState GAMESTATE;

#endif // __GameState_H__