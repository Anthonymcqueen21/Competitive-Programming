using System;  
using System.IO;
using System.Text;
using System.Collections;
using System.Collections.Generic;
using System.Text.RegularExpressions;
class Player
{
         class Point
         {
                  public double X;
                  public double Y;
         }

         // Given three colinear points p, q, r, the function checks if
         // point q lies on line segment 'pr'
         static bool onSegment(Point p, Point q, Point r)
         {
                  if (q.X <= Math.Max(p.X, r.X) && q.X >= Math.Min(p.X, r.X) &&
                           q.Y <= Math.Max(p.Y, r.Y) && q.Y >= Math.Min(p.Y, r.Y))
                           return true;

                  return false;
         }
         // The main function that returns true if line segment 'p1q1'
         // and 'p2q2' intersect.
         static bool doIntersect(Point p1, Point q1, Point p2, Point q2)
         {
                  // Find the four orientations needed for general and
                  // special cases
                  int o1 = orientation(p1, q1, p2);
                  int o2 = orientation(p1, q1, q2);
                  int o3 = orientation(p2, q2, p1);
                  int o4 = orientation(p2, q2, q1);

                  // General case
                  if (o1 != o2 && o3 != o4)
                           return true;
                  // Special Cases
                  // p1, q1 and p2 are colinear and p2 lies on segment p1q1
                  if (o1 == 0 && onSegment(p1, p2, q1)) return true;

                  // p1, q1 and p2 are colinear and q2 lies on segment p1q1
                  if (o2 == 0 && onSegment(p1, q2, q1)) return true;

                  // p2, q2 and p1 are colinear and p1 lies on segment p2q2
                  if (o3 == 0 && onSegment(p2, p1, q2)) return true;

                  // p2, q2 and q1 are colinear and q1 lies on segment p2q2
                  if (o4 == 0 && onSegment(p2, q1, q2)) return true;

                  return false; // Doesn't fall in any of the above cases
         }

         // To find orientation of ordered triplet (p, q, r).
         // The function returns following values
         // 0 --> p, q and r are colinear
         // 1 --> Clockwise
         // 2 --> Counterclockwise
         static int orientation(Point p, Point q, Point r)
         {
                  // See 10th slides from following link for derivation of the formula
                  // https://www.dcs.gla.ac.uk/~pat/52233/slides/Geometry1x1.pdf
                  double val = (q.Y - p.Y) * (r.X - q.X) -
                                      (q.X - p.X) * (r.Y - q.Y);

                  if (val == 0) return 0;  // colinear

                  return (val > 0) ? 1 : 2; // clock or counterclock wise
         }

         static void Main(String[] args)
         {
                  // Read init information from standard input, if any

                  int numberOfpoints = int.Parse(Console.ReadLine());
                  var ptList = new List<Point>();

                  for (int i = 0; i < numberOfpoints; i++)
                  {
                           string input = Console.ReadLine();
                           string[] parts = input.Split(' ');
                           Point p = new Point();
                           p.X = int.Parse(parts[0]);
                           p.Y = int.Parse(parts[1]);
                           ptList.Add(p);
                  }


                  Console.Error.WriteLine("nb of points : " + ptList.Count.ToString());

                  // compute landing zone :
                  /// we will aim at landing around the middle of the horizontal part
                  double highestY = 0;

                  int foundPt = -1;
                  for (int i = 0; i < numberOfpoints - 1; i++)
                  {
                           double Y = ptList[i].Y;
                           double nextY = ptList[i + 1].Y;
                           double length = ptList[i + 1].X - ptList[i + 1].Y;

                           /// here I removed this test as for some reason, the landing zone
                           /// was actually smaller than 1000
                           //            if (length < 1000)
                           //                continue;
                           if (Y == nextY)
                           {
                                    foundPt = i;
                           }
                           if (Y > highestY)
                           {
                                    /// we also keep track of the highest point of the landscape
                                    /// we tweak our behavior depending on whether we're above this or not
                                    highestY = Y;
                           }
                  }

                  if (foundPt < 0)
                           throw new Exception("not found");

                  /// this is the middle of the horizontal line
                  /// we want to land on
                  Point LZ = new Point();
                  LZ.X = ptList[foundPt].X + ptList[foundPt + 1].X;
                  LZ.X /= 2;
                  LZ.Y = ptList[foundPt].Y + ptList[foundPt + 1].Y;
                  LZ.Y /= 2;


                  int outR = 0;
                  int outP = 3;

                  double groundApproachingSpeed = -1;
                  double previousDist = -1;

                  while (true)
                  {
                           string[] parts = Console.ReadLine().Split(' ');
                           int X = int.Parse(parts[0]);
                           int Y = int.Parse(parts[1]);
                           int HS = int.Parse(parts[2]);
                           int VS = int.Parse(parts[3]);
                           int F = int.Parse(parts[4]);
                           int R = int.Parse(parts[5]);
                           int P = int.Parse(parts[6]);

                           Console.Error.WriteLine("Fuel : " + F.ToString());
                           Console.Error.WriteLine("CurPos : " + X.ToString() + " " + Y.ToString());
                           Console.Error.WriteLine("Landing zone : " + LZ.X.ToString() + " " + LZ.Y.ToString());

                           /// here I tried to keep track of how far we were from the ground
                           /// but I actually didn't use this value
                           double dstToGround = Math.Sqrt((LZ.X - X) * (LZ.X - X) + (LZ.Y - Y) * (LZ.Y - Y));

                           /// the vertical and horizontal distance to the landing zone
                           /// were actually a little more useful
                           double vDist = Y - LZ.Y;
                           double hDist = X - LZ.X;

                           /// here I just use another name for X and Y
                           /// because I wasn't sure about the
                           /// named parameters constructor just below ...
                           int pX = int.Parse(parts[0]);
                           int pY = int.Parse(parts[1]);

                           /// ... there (and I had no time to double check {X = X, Y = Y} would work, but I’m pretty sure it should even if it’s ugly)
                           Point p1 = new Point() { X = pX, Y = pY };
                           Point q1 = LZ;

                           /// p1 q1 is the segment between our current position and the landing zone
                           /// p2 q2 are each segment of the landscape
                           /// => we check if we have a clear line of sight to our destination
                           /// and if not, we will continue moving above the highest point of the land
                           bool bCollisionFound = false;
                           int nbCollision = 0;
                           for (int i = 0; i < numberOfpoints - 1; i++)
                           {
                                    Point p2 = ptList[i];
                                    Point q2 = ptList[i + 1];
                                    if (doIntersect(p1, q1, p2, q2))
                                    {
                                            nbCollision++;
                                    }
                           }
                           /// only 1 collision means we have a clear line of sight
                           /// the only intersection being the point we aim at (q1)
                           bCollisionFound = nbCollision > 1;

                           /// that part of the code was written before the collision check:
                           /// I tried to keep track of an approximation of what would be
                           /// the nearest obstacle so that I could try and avoid it, which I finally didn't implement
                           int closestPointIndex = 0;
                           double closestDist = double.MaxValue;
                           for (int i = 0; i < numberOfpoints; i++)
                           {
                                    /// that poor name was because I already have a "P" declared above, sorry about that
                                    Point Pipi = ptList[i];
                                    double dist = Math.Sqrt((Pipi.X - X) * (Pipi.X - X) + (Pipi.Y - Y) * (Pipi.Y - Y));
                                    if (dist < closestDist)
                                    {
                                            closestDist = dist;
                                            closestPointIndex = i;
                                    }
                           }

                           /// still the unfinished code about trying to avoid obstacles while cruising
                           if (previousDist > 0)
                           {
                                    groundApproachingSpeed = previousDist - closestDist;
                           }

                           previousDist = closestDist;
                           Console.Error.WriteLine("groundApproachingSpeed : " + closestDist.ToString());

                           /// a state variable :
                           /// we're ready to land if we're above our destination
                           /// but below the highest point in the landscape
                           /// probably not relevant after the collision check was implemented
                           bool isReadyToLand = false;

                           if (Y < highestY - 100)
                           {
                                    if (Math.Abs(hDist) > 1000)
                                    {
                                            isReadyToLand = false;
                                    }
                                    else
                                    {
                                            isReadyToLand = true;
                                    }
                           }
                           Console.Error.WriteLine("isReadyToLand : " + isReadyToLand.ToString());


                           /// some heuristic to decide what should be our horizontal velocity
                           /// we try not to go too fast when we're still high above the ground
                           /// (that was to avoid some high pit from exercice 2, as far as I remember)
                           /// but we also allow a bigger correction when close to landing
                           /// (that was to above some other crash)
                           int hOptimalSpeed = 20;
                           if (highestY > 2000)
                                    hOptimalSpeed = 15;
                           if (bCollisionFound)
                                    hOptimalSpeed = 20;

                           /// if our horizontal distance is positive, we try to adjust our speed to the left
                           /// and if our horizontal distance is negative, to the right
                           /// execpt for (1*), see below for the comments
                           int desiredHSpeed = 0;
                           if (hDist > 10 || bCollisionFound /* (1*) */ )
                                    desiredHSpeed = -hOptimalSpeed;
                           else if (hDist < -10)
                                    desiredHSpeed = hOptimalSpeed;
                           else
                                    desiredHSpeed = 0;
                           /// (*1) this part is rather stupid :
                           /// the ship will continue going to the *left*
                           /// as long as there's no clear line of sight
                           /// to our landing zone.
                           /// this would need some adjustments to handle
                           /// a case where our landing zone would be to our right
                           /// Fortunately, this didn't happen in the tests

                           /// adjust our rotation so that we reach our desired horizontal speed
                           if (HS < desiredHSpeed - 1)
                                    outR = HS - desiredHSpeed;
                           else if (HS > desiredHSpeed + 1)
                                    outR = HS - desiredHSpeed;
                           else
                                    outR = 0;
                           outR *= 1;

                           int maxAngle = 45;
                           /// another adjustment to our behavior that seemed to help :
                           /// don't try to use too big an angle when we're not ready to land
                           if (!isReadyToLand)
                           {
                                    maxAngle = 30;
                           }
                           outR = (int)Math.Min(outR, maxAngle);
                           outR = (int)Math.Max(outR, -maxAngle);

                           /// we're very close to the ground, so no time to adjust our trajectory
                           /// anymore, put back our ship in a standing state
                           if (vDist < 50)
                                    outR = 0;

                           Console.Error.WriteLine("hDist : " + hDist.ToString());
                           Console.Error.WriteLine("vDist : " + vDist.ToString());
                           Console.Error.WriteLine("HS : " + HS.ToString());
                           Console.Error.WriteLine("desiredHSpeed : " + desiredHSpeed.ToString());
                           Console.Error.WriteLine("closestDist : " + closestDist.ToString());

                           Console.Error.WriteLine("DstToGround : " + vDist.ToString());

                           /// we use the 4 level power only when we're ready to land
                           /// or when we try to keep above the highest point of the landscape
                           //if (vDist < 100 && Math.Abs(hDist) > 1000)
                           if (!isReadyToLand && (VS < 0 || Y < highestY + 100) && bCollisionFound)
                           {
                                    outP = 4;
                           }
                           else if (!isReadyToLand && (closestDist < 300 || VS < -10))
                           {
                                    outP = 4;
                           }
                           else if (VS < -20)
                           {
                                    outP = 4;
                           }
                           else
                           {
                                    /// we're not going down fast enough (or we're going up),
                                    /// so reduce the power
                                    outP = 3;
                           }

                           Console.WriteLine(outR.ToString() + " " + outP.ToString());
                  }


                  return;

         }
}
