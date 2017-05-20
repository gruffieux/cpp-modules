#ifndef CMOVER
#define CMOVER

#include <citem.h>

/*
Bugs:
Rien à signaler!!
*/

class Mover : virtual public Item
{
private:
	double acceleration, deceleration;
	CardinalPoint BlockedDirection, CurrentObstacle;
protected:
	double magnitude, angle;
	CardinalPoint Direction;
public:
	Mover() {angle = acceleration = deceleration = magnitude = 0;}
	Mover(double Mover_angle, double Mover_acc, double Mover_dec);
	Mover(Mover &model);
	~Mover() {}
	void SetAngle(double Mover_angle) {angle = Mover_angle;}
	void SetAcceleration(double Mover_acceleration) {acceleration = Mover_acceleration;}
	void SetDeceleration(double Mover_deceleration) {deceleration = Mover_deceleration;}
	void SetMagnitude(double Mover_magnitude) {magnitude = Mover_magnitude;}
	void GenerateMouseAngle(int x, int y);
	void IncreaseAngle(double min, double max, int total);
	void DecreaseAngle(double min, double max, int total);
	void IncreaseMagnitude(int max);
	void DecreaseMagnitude(int max);
	void GenerateDirection();
	CardinalPoint * GenerateCollision(CardinalPoint *Obstacle1, CardinalPoint Obstacle2[], int ObstacleCount, bool stop, bool rebound);		//Gère les collisions n'entourant pas l'objet
	void GenerateCollision(int x, int y, CardinalPoint *Obstacle, bool stop);																//Gère les collisions entourant l'objet
	void Move(CardinalPoint *MaxDistance=NULL, bool stop=false, bool rebound=false);																			//Mouvement pour objet dirigé au clavier
	void Move(int x, int y, CardinalPoint *MaxDistance=NULL, bool stop=false);																			//Mouvement pour la souris
	virtual bool StopMove();
	void ReverseAngle(bool left, bool right, bool up, bool down);
	bool hasNoDirection() {return !Direction.west && !Direction.east && !Direction.north && !Direction.south;}
	double GetDegreeAngle(int subtracter) {return angle * 180 / PI - subtracter;}
	double GetAngle() {return angle;}
	double GetAcceleration() {return acceleration;}
	double GetDeceleration() {return deceleration;}
	double GetMagnitude() {return magnitude;}
	CardinalPoint * GetDirection() {return &Direction;}
	CardinalPoint * GetBlockedDirection() {return &BlockedDirection;}
	CardinalPoint * GetCurrentObstacle() {return &CurrentObstacle;}
};

#endif