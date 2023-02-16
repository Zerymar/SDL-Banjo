#pragma once
#include <SDL_rect.h>
#include <stack>
#include <vector>
#include <math.h>


enum ORIENTATION
{
    CounterClockwise = -1,
    Collinear = 0,
    Clockwise = 1
};

class Geometry
{
    static SDL_FPoint point0;
public:
    
    
    // Utility function to find next to top in Stack
    static SDL_FPoint NextToTop(std::stack<SDL_FPoint> &stack)
    {
        SDL_FPoint top = stack.top();
        stack.pop();
        SDL_FPoint next = stack.top();
        stack.push(top);
        return next;
    }

    // Utility for swapping coordinates
    static void SwapPoints(SDL_FPoint& p_a, SDL_FPoint& p_b)
    {
        SDL_FPoint temp = p_a;
        p_a = p_b;
        p_b =temp;
    }

    static double EuclideanDistance(SDL_FPoint p_a, SDL_FPoint p_b)
    {
        float x = p_a.x - p_b.x;
        float y = p_a.y - p_b.y;
        double distance  = pow(x, 2) + pow(y, 2);
        return sqrt(distance);
    }
    
    static double DistanceSquared(SDL_FPoint p_a, SDL_FPoint p_b)
    {
        return pow(EuclideanDistance(p_a, p_b), 2);
    }

    // Find ordered orientation of triplet (a,b,c)
    static ORIENTATION FindOrientation(SDL_FPoint p_a, SDL_FPoint p_b, SDL_FPoint p_c)
    {
        float value = (p_b.y - p_a.y) *(p_c.x - p_b.x) -
            (p_b.x - p_a.x)* (p_c.y - p_b.y);
        if(value == 0)
        {
            return Collinear;
        }
        return value > 0 ? Clockwise : CounterClockwise;
        
    }

    // function to be used by qsort in order to sort our points
    // Point0
    static int compare(void const *vp_a, void const *vp_b)
    {
        SDL_FPoint *p_a = (SDL_FPoint *) vp_a;
        SDL_FPoint *p_b = (SDL_FPoint *) vp_b;

        // Find orientation
        ORIENTATION orientation = FindOrientation(point0, *p_a, *p_b);
        if(orientation == Collinear)
        {
            return (DistanceSquared(point0, *p_b) >= DistanceSquared(point0, *p_a))? -1 : 1;
        }
        return (orientation == CounterClockwise) ? -1 : 1;
    }
    

    static void ConvexHull(std::vector<SDL_FPoint>& convexHullPoints, std::vector<SDL_FPoint> points)
    {
        int n = static_cast<int>(points.size());

        //find the bottom most point
        float y_Min = points[0].y;
        int minIndex = 0;
        for(int i =1; i < n; ++i)
        {
            float yCoord = points[i].y;

            // Pick bottom most or choose the left in case of tie
            if((yCoord < y_Min) || (y_Min == yCoord &&
                points[i].x < points[minIndex].x))
            {
                y_Min = points[i].y;
                minIndex =i;
            }
        }

        SwapPoints(points[0], points[minIndex]);
        //initialize our p0 and sort points
        point0 = points[0];
        qsort(&points[1], n-1, sizeof(SDL_FPoint), compare);
        // If two or more points make same angle with p0,
        // Remove all but the one that is farthest from p0
        // Remember that, in above sorting, our criteria was
        // to keep the farthest point at the end when more than
        // one points have same angle.
        
        // Reuse Points for our modified array, set the size
        int m =1;
        for(int i = 1; i < n; ++i)
        {
            // keep moving up our i while angle of i, i+1 is same with respect to point0
            // we will be skipping the points
            while(i < n-1 && FindOrientation(point0, points[i], points[i+1]) == Collinear){++i;}

            points[m] = points[i];
            ++m;
        }

        // we're done if we dont have 3 points, no shape to form
        if(m < 3) return;

        // clear out our vector
        convexHullPoints.clear();
        
        std::stack<SDL_FPoint> stack;
        stack.push(points[0]);
        stack.push(points[1]);
        stack.push(points[2]);

        // process the remaining n-3 points
        for(int i = 3; i < m; ++i)
        {
            // keep removing depending on the angle formed by top, next top, points[i]
            while(stack.size()>1 &&
                FindOrientation(NextToTop(stack), stack.top(), points[i]) != CounterClockwise)
            {
                stack.pop();
            }
            stack.push(points[i]);
        }

        while(!stack.empty())
        {
            SDL_FPoint point = stack.top();
            convexHullPoints.push_back(point);
            stack.pop();
        }
        
    }
};
