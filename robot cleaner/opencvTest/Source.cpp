#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <stdio.h>
#include "Utilities.h"
//for the clear console command
#include <cstdlib>
//#include "opencv2/video.hpp"
// for measuring speed of execution
#include <time.h>
#include <math.h>  
#define PI 3.14159265


using namespace std;
using namespace cv;


void whiteRect(Mat& image, int x1, int y1, int x2, int y2)
{
	rectangle(image, Point(x1, y1), Point(x2, y2), Scalar(255, 255, 255), -1, 8);
}

void drawHuman(Mat& image, int x1, int y1)
{
	Point circleCenter;
	circleCenter.x = x1;
	circleCenter.y = y1;
	circle(image, circleCenter, 3, Scalar(0, 0, 255), -1);
}

bool testCollide(Mat collisionView, Point& Humanposition)
{
	//cout << Humanposition.y << "," << Humanposition.x << ".";
	Scalar intensity = collisionView.at<Vec3b>(Humanposition.y, Humanposition.x);
	//cout << "->" << intensity.val[0] << "      ";
	//cout << intensity.val[0];

	if (intensity.val[0] == 255)
		return false;
	else
		return true;

}

void path_fragment(Mat& image, Point start, Point finish, Scalar color = Scalar(0, 200, 0))
{
	line(image, start, finish, color, 1, 8);
}

void moveHuman(int XtoMove, int YtoMove, Mat collisionView, Point& Humanposition)
{
	//cout << "c";

	int max_distance;
	if (abs(XtoMove) > abs(YtoMove))
		max_distance = XtoMove;
	else
		max_distance = YtoMove;

	int counterX = XtoMove;
	int counterY = YtoMove;

	for (int i = 0; i < abs(max_distance); i++)
	{
		Mat tempCollision = collisionView.clone();
		//if (collisionView.at<Vec3b>(70, 120 - i)[1] != 255)
		if (testCollide(collisionView, Humanposition) == false)
		{
			//change human position in memory
			//Humanposition.x = 300 - i;
			//Humanposition.y = 70 + i;
			Point test;

			if (counterX != 0)
			{
				if (counterX > 0)
				{
					test.x = Humanposition.x+1;
					test.y = Humanposition.y;
					if (testCollide(collisionView, test) == false)
						Humanposition.x = Humanposition.x + 1;
					counterX--;
				}
				else
				{
					test.x = Humanposition.x - 1;
					test.y = Humanposition.y;
					if (testCollide(collisionView, test) == false)
					Humanposition.x = Humanposition.x - 1;
					counterX++;
				}
			}
			if (counterY != 0)
			{
				if (counterY > 0)
				{
					test.x = Humanposition.x;
					test.y = Humanposition.y + 1;
					if (testCollide(collisionView, test) == false)
					Humanposition.y = Humanposition.y + 1;
					counterY--;
				}
				else
				{
					test.x = Humanposition.x;
					test.y = Humanposition.y - 1;
					if (testCollide(collisionView, test) == false)
					Humanposition.y = Humanposition.y - 1;
					counterY++;
				}
			}

			//Humanposition.x = Humanposition.x - 1;

			//Humanposition.y = Humanposition.y + 1;
			//draw it at new position


		}
		/*
		Point p1, p2;
		p1.x = 0; p1.y = 0;
		p2.x = 1000; p2.y = 1000;
		line(tempCollision, p1, p2, Scalar(0, 200, 0), 5, 8);*/
		path_fragment(tempCollision,Point(250,90),Point(250,210));
		path_fragment(tempCollision, Point(250, 210), Point(700,220),Scalar(200,0,0));


		drawHuman(tempCollision, Humanposition.x, Humanposition.y);
		imshow("collision view", tempCollision);
		//moveWindow("collision view", 600, 100);
		waitKey(10);
	}
	
}



double euclidian_dist(double x1, double y1, double x2, double y2)
{
	double x = x1 - x2; //calculating number to square in next step
	double y = y1 - y2;
	double dist;

	dist = pow(x, 2) + pow(y, 2);       //calculating Euclidean distance
	dist = sqrt(dist);

	return dist;
}

struct Path
{
	Point start_fragment;
	vector<Point> next_points;
};

void draw_all_paths(vector<Path> paths, Mat& img_to_draw_on)
{
for (int j = 0; j < paths.size() /*paths[].size*/; j++)
{
	for (int i = 1; i < paths[j].next_points.size()/*(sizeof(end_points) / sizeof(*end_points))*/ + 1; i++)
	{
		int b, g, r; b = 0; g = 0; r = 0;
		switch (i)
		{
		case 1:
		{
			g = 200;
			break;
		}
		case 2:
		{
			b = 200; g = 200;
			break;
		}
		case 3:
		{
			g = 200; r = 200;
			break;
		}
		case 4:
		{
			b = 200; r = 200;
			break;
		}
		default:
		{
			b = 66; g = 66; r = 66;
			break;
		}
		}
		if (i == 1)
			path_fragment(img_to_draw_on, Point(paths[j].start_fragment.x, paths[j].start_fragment.y), Point(paths[j].next_points[0].x, paths[j].next_points[0].y), Scalar(b, g, r));
		//else // 2 3 4 ...
		if (i > 1)
		{
			path_fragment(img_to_draw_on, Point(paths[j].next_points[i - 2].x, paths[j].next_points[i - 2].y), Point(paths[j].next_points[i - 1].x, paths[j].next_points[i - 1].y), Scalar(b, g, r));
			int kk = 3;
		}
	}
}
}




class robot {

private:
	Rect starting_coordinates;
	Rect current_coordinates;
	int battery_timer;
	float battery_level;

public:

	// Constructeur sans paramètre (constructeur par défaut)
	robot(int x = 400, int y = 150, int size = 40)
	{
		starting_coordinates.x = x;
		starting_coordinates.y = y;
		starting_coordinates.height = size;
		starting_coordinates.width = size;
		current_coordinates.x = starting_coordinates.x;
		current_coordinates.y = starting_coordinates.y;
		current_coordinates.height = starting_coordinates.height;
		current_coordinates.width = starting_coordinates.width;
		battery_timer = 0;
		battery_level = 100;

	}

	int get_robot_size()
	{
		return starting_coordinates.height;
	}
	// Constructeur avec un paramètre
	/*robot(double no_use) 
	{
		starting_coordinates.x = 150;
		starting_coordinates.y = 150;
		starting_coordinates.height = 10;
		starting_coordinates.width = 10;
	}*/

	// Méthodes d'accès à l'attribut moyenne
	Rect get_starting_Coords() 
	{ 
		return starting_coordinates;
	}
	Rect get_current_Coords()
	{
		return current_coordinates;
	}

	void change_current_Coords(int X, int Y) 
	{
		current_coordinates.x = current_coordinates.x + X;
		current_coordinates.y = current_coordinates.y + Y;
	}
	int get_battery()
	{
		return battery_level;
	}
	void change_battery()
	{
		battery_timer++;
		battery_level = (100 - (battery_timer / 10));
		if (battery_level < 0)
			battery_level = 0;
	}

};

bool check_collision(Mat image_under_robot)
{
	bool result = true;
	for (int col = 0; col < image_under_robot.cols; col++)
	{
		for (int row = 0; row < image_under_robot.rows; row++)
		{
			Scalar intensity = image_under_robot.at<Vec3b>(row, col);
			if (intensity.val[0] != 255)
				result = false;
		}
	}
	return result;
}

//extract the passable areas of the image (255 white) and set everything else to black.
//a threshold function may be faster, but this one may do more precisely what I need.
Mat extract_background(Mat room_image)
{
	Mat canvas = room_image.clone();
	for (int col = 0; col < canvas.cols; col++)
	{
		for (int row = 0; row < canvas.rows; row++)
		{
			Scalar intensity = room_image.at<Vec3b>(row, col);
			if (intensity.val[0] != 255)//non white = non passableset to black
				canvas.at<Vec3b>(row, col) = 0;
			else//passable, make it gray (for dirty)
			{
				Vec3b inject(200,200,200);
				canvas.at<Vec3b>(row, col) = inject;
			}
				
		}
	}
	return canvas;
}

void print_value(int value_collection, Mat& image_to_draw_on, int draw_at_X, int draw_at_Y)
{
	stringstream strs;
	strs << value_collection;
	string temp_str = strs.str();
	char* char_type = (char*)temp_str.c_str();
	writeText(image_to_draw_on, char_type, draw_at_X, draw_at_Y, Scalar(0, 0, 0), 0.5, 1);
}
void print_value(float value_collection, Mat& image_to_draw_on, int draw_at_X, int draw_at_Y)
{
	stringstream strs;
	strs << value_collection;
	string temp_str = strs.str();
	char* char_type = (char*)temp_str.c_str();
	writeText(image_to_draw_on, char_type, draw_at_X, draw_at_Y, Scalar(0, 0, 0), 0.5, 1);
}

//draw random direction line
vector<Point> show_direction(Mat& image, Rect robot_coords, double chosen_angle)
{
	Point roomba_coord;
	vector<Point> path;

	/*roomba_coord.x = image.rows / 2;
	roomba_coord.y = image.cols / 2;*/
	roomba_coord.x = robot_coords.y;
	roomba_coord.y = robot_coords.x;

	Point max_roomba_coord;
	//max_roomba_coord.x = roomba_coord.x;

	int quart;
	//double angle = rand() % 360;
	
	double angle = chosen_angle;
	if (angle <= 89)
		quart = 1;
	else
		if (angle <= 179)
			quart = 2;
		else
			if (angle <= 269)
				quart = 3;
			else
				quart = 4;

	int h = image.rows;
	int l = image.cols;
	for (double adjacent = 0; adjacent <= image.rows; adjacent++)
	{
		double hypotenuse = adjacent / cos(angle * PI / 180.0);
		double opposite = sqrt((hypotenuse * hypotenuse) - (adjacent * adjacent));
		int trunk_opposite = (int)opposite;
		switch (quart)
		{
		case 1:
			max_roomba_coord.x = roomba_coord.x + adjacent;
			max_roomba_coord.y = roomba_coord.y + trunk_opposite;
			break;
		case 2:
			max_roomba_coord.x = roomba_coord.x - adjacent;
			max_roomba_coord.y = roomba_coord.y + trunk_opposite;
			break;
		case 3:
			max_roomba_coord.x = roomba_coord.x - adjacent;
			max_roomba_coord.y = roomba_coord.y - trunk_opposite;
			break;
		case 4:
			max_roomba_coord.x = roomba_coord.x + adjacent;
			max_roomba_coord.y = roomba_coord.y - trunk_opposite;
			break;
		}
		//cout << "adjacent : " << adjacent << " X: " << max_roomba_coord.x << " Y: " << max_roomba_coord.y << endl;

		if (max_roomba_coord.x < image.rows && max_roomba_coord.y < image.cols && max_roomba_coord.x >= 0 && max_roomba_coord.y >= 0)
		{
			/*image.at<Vec3b>(max_roomba_coord.x, max_roomba_coord.y)[0] = 0;
			image.at<Vec3b>(max_roomba_coord.x, max_roomba_coord.y)[1] = 0;
			image.at<Vec3b>(max_roomba_coord.x, max_roomba_coord.y)[2] = 0;*/
			path.push_back(Point(max_roomba_coord.x, max_roomba_coord.y));
		}
	}
	return path;
}


//create a list of nodes at random positions on the room image, then show them
void view_nodes(Mat& room, vector<Rect>& nodes, robot this_robot)
{
	clock_t local_timer;
	local_timer = clock();

	//this temp view is used to avoid superposing node spots in top of each other, then drawing paths on it too.
	Mat node_view = room.clone();
	

	//generate random position
	

	Rect local_coords = this_robot.get_current_Coords();


	//clone of the empty room image, used to check paths for "passability"
	Mat room_unaltered = room.clone();

	Mat paths_test_temp = room.clone();

	//depending on the starting room image (enough white space) this should create from 0 to ~50 nodes in the scene.
	for (int tries = 0; tries < 20; tries++)
	{
		int rand_x = rand() % (node_view.cols - local_coords.width);
		int rand_y = rand() % (node_view.rows - local_coords.height);


		Rect area_of_interest;
		area_of_interest.x = rand_x;
		area_of_interest.y = rand_y;
		area_of_interest.height = this_robot.get_robot_size();
		area_of_interest.width = this_robot.get_robot_size();
		Mat frag_check = node_view(area_of_interest);

		//imshow("frag check", frag_check);

		Mat white_area = Mat::zeros(frag_check.rows, frag_check.cols, frag_check.type());
		white_area.setTo(cv::Scalar(255, 255, 255));

		Mat diff;
		cvtColor(white_area, white_area, CV_BGR2GRAY);
		cvtColor(frag_check, frag_check, CV_BGR2GRAY);
		compare(white_area, frag_check, diff, cv::CMP_NE);
		int nz = countNonZero(diff);



		//Scalar intensity = room.at<Vec3b>(rand_y, rand_x);
		if (nz == 0)//intensity.val[1] == 255 && nz == 0)
		{

			Rect node_area;
			node_area.x = rand_x;
			node_area.y = rand_y;
			node_area.height = this_robot.get_robot_size();
			node_area.width = this_robot.get_robot_size();
			//debug : show starting area for nodes
			//rectangle(node_view, node_area, Scalar(0, 0, 0), 1, 8);

			Point node_Center;
			node_Center.x = rand_x + (this_robot.get_robot_size() / 2);
			node_Center.y = rand_y + (this_robot.get_robot_size() / 2);
			circle(node_view, node_Center, 6, Scalar(0, 0, 255), -1);

			nodes.push_back(node_area);//create a list of all the nodes we decided to keep
		}
	}


	//This container stores, for each node, all the other nodes it is connected to.
	//to be fair, a cleaner version would be to make a struct contained the vector of nodes and this container.

	vector< vector<int> > connexion_between_nodes;
	

	if (nodes.size() > 1)//2 nodes min
		for (int current_node = 0; current_node < nodes.size(); current_node++)
		{
			vector<int> nodes_connected_to_this_node;
			for (int other_node = current_node; other_node < nodes.size(); other_node++)
			{
				
				if (current_node != other_node)
				{
					//drawing all paths including the impossible ones in grey
					//line(node_view, Point(nodes[current_node].x + (this_robot.get_robot_size() / 2), nodes[current_node].y + (this_robot.get_robot_size() / 2)), Point(nodes[other_node].x + (this_robot.get_robot_size() / 2), nodes[other_node].y + (this_robot.get_robot_size() / 2)), Scalar(170,170,170), 1, 8);
					//checking the paths for passability and drawing the passable ones in a brighter color
					int explor_x = nodes[current_node].x;
					int explor_y = nodes[current_node].y;//start at one node's coordinates
					int reach_x = nodes[other_node].x;
					int reach_y = nodes[other_node].y;//obtain destination node's coordinates
					bool fragment_is_passable = true;
					//check this fragment for "passability"
					do
					{
						//insert test here
						Rect area_of_interest;
						area_of_interest.x = explor_x;
						area_of_interest.y = explor_y;
						area_of_interest.height = this_robot.get_robot_size();
						area_of_interest.width = this_robot.get_robot_size();
						Mat frag_check = room_unaltered(area_of_interest);

						/*imshow("frag check", frag_check);
						waitKey();*/

						Mat white_area = Mat::zeros(frag_check.rows, frag_check.cols, frag_check.type());
						white_area.setTo(cv::Scalar(255, 255, 255));

						Mat diff;
						cvtColor(white_area, white_area, CV_BGR2GRAY);
						cvtColor(frag_check, frag_check, CV_BGR2GRAY);
						compare(white_area, frag_check, diff, cv::CMP_NE);
						int nz = countNonZero(diff);

						if (nz != 0)
							fragment_is_passable = false;

						//get closer to reach position using at maximum one pixel movement per axis
						//that movement isn't a straight line
						if (explor_x < reach_x) explor_x++;
						if (explor_x > reach_x) explor_x--;
						if (explor_y < reach_y) explor_y++;
						if (explor_y > reach_y) explor_y--;

						//debug
						/*if (nz != 0)
							rectangle(paths_test_temp, area_of_interest, Scalar(0, 200, 0), -1);
						else
							rectangle(paths_test_temp, area_of_interest,Scalar(0,0,0),-1);*/

					} while (explor_x != reach_x || explor_y != reach_y);

					//draw the fragment for passability, if passable
					/*if (fragment_is_passable == true)
						line(node_view, Point(nodes[current_node].x + (this_robot.get_robot_size() / 2), nodes[current_node].y + (this_robot.get_robot_size() / 2)), Point(nodes[other_node].x + (this_robot.get_robot_size() / 2), nodes[other_node].y + (this_robot.get_robot_size() / 2)), Scalar(255, 0, 0), 1, 8);*/
					if (fragment_is_passable == true)
					{
						//add the ref. number of this node as a connection
						nodes_connected_to_this_node.push_back(other_node);
						int explor_x = nodes[current_node].x;
						int explor_y = nodes[current_node].y;//start at one node's coordinates
						int reach_x = nodes[other_node].x;
						int reach_y = nodes[other_node].y;//obtain destination node's coordinates
						do
						{
							Rect point_area;
							point_area.x = explor_x + (this_robot.get_robot_size() / 2);
							point_area.y = explor_y + (this_robot.get_robot_size() / 2);
							point_area.height = 1;
							point_area.width = 1;
							rectangle(node_view, point_area, Scalar(200, 0, 0), -1);
							if (explor_x < reach_x) explor_x++;
							if (explor_x > reach_x) explor_x--;
							if (explor_y < reach_y) explor_y++;
							if (explor_y > reach_y) explor_y--;
						} while (explor_x != reach_x || explor_y != reach_y);
					}
						

				}
				
			}
			connexion_between_nodes.push_back(nodes_connected_to_this_node);
		}

	//redraw the nodes so they could be visually "on top" of the blue paths
	for (int current_node = 0; current_node < nodes.size(); current_node++)
	{
		Point node_Center;
		node_Center.x = nodes[current_node].x + (this_robot.get_robot_size() / 2);
		node_Center.y = nodes[current_node].y + (this_robot.get_robot_size() / 2);
		//drawing the node in the form of a filled red circle
		circle(node_view, node_Center, 6, Scalar(0, 0, 255), -1);

		stringstream strs;
		strs << current_node/*+1*/;
		string temp_str = strs.str();
		char* char_type = (char*)temp_str.c_str();
		putText(node_view, char_type, Point(node_Center.x-8, node_Center.y), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0,0,0), 2);
	}

	//connexions are one way only, we want to make them both way.
	// visual example : node 1 is connected to 7 in the incoming list. we want after that module that 7 is connected to 1 too.
	//vector< vector<int> > connexion_between_nodes_2way;
	//for the first node, information is correct, clone it
	//vector<int> temp_local_list = connexion_between_nodes[0];// first node's list
	//connexion_between_nodes_2way.push_back(temp_local_list);
	//start at second node

	vector<Point> pairs;
	for (int this_node_s_connexions = 0; this_node_s_connexions < connexion_between_nodes.size(); this_node_s_connexions++)
	{

		//	//if (this_node_s_connexions != connexion)//don't do something with itself
		//		//if (connexion_between_nodes[this_node_s_connexions][connexion] > this_node_s_connexions)


		vector<int> temp_local_list = connexion_between_nodes[this_node_s_connexions];

		for (int connexion = 0; connexion < temp_local_list.size(); connexion++)
		{
			//connexion_between_nodes[connexion].push_back(50);
			cout << "[" << temp_local_list[connexion] << "," << this_node_s_connexions << "]";
			pairs.push_back(Point(temp_local_list[connexion], this_node_s_connexions));

		}

		for (int connexion = 0; connexion < temp_local_list.size(); connexion++)
		{
			//connexion_between_nodes[this_node_s_connexions].push_back(70);
		}



		//connexion_between_nodes[this_node_s_connexions].push_back(99);
	}
	//connexion_between_nodes[2].push_back(99);
	cout << endl;

	for (int current_pair = 0; current_pair < pairs.size(); current_pair++)
	{
		Point pair;
		pair = pairs[current_pair];

		cout << "[" << pair.x << "," << pair.y << "]";
		//connexion_between_nodes[pair.x].push_back(222);
		connexion_between_nodes[pair.x].push_back(pair.y);
	}

	//showing node connexion list
	cout << endl;
	for (int this_node_s_connexions = 0; this_node_s_connexions < connexion_between_nodes.size(); this_node_s_connexions++)
	{
		cout << "node " << this_node_s_connexions/*+1*/ << " : ";
		vector<int> temp_local_list = connexion_between_nodes[this_node_s_connexions];
		for (int connexion = 0; connexion < temp_local_list.size(); connexion++)
		{
			cout << temp_local_list[connexion]/*+1*/ << " ";
		}
		cout << endl;
	}
	

	//checking computing time
	local_timer = clock() - local_timer;
	cout << "path calculating time : " << (float)local_timer << "ms ";

	imshow("node view", node_view);
	//imshow("tested paths", paths_test_temp);
	waitKey();

}


int main(int argc, const char** argv)
{


	/*********************** background pic loading ********************************/
	string name_bg = "C:\\Users\\User\\Desktop\\roomba\\media\\room4.png";
	// current image is loaded
	Mat room = imread(name_bg);
	string name_visual_bg = "C:\\Users\\User\\Desktop\\roomba\\media\\room3C.jpg";
	// current image is loaded
	Mat room_visual = imread(name_visual_bg);
	string name_robot = "C:\\Users\\User\\Desktop\\roomba\\media\\robot.jpg";
	// current image is loaded
	Mat robot_image = imread(name_robot);
	string name_base = "C:\\Users\\User\\Desktop\\roomba\\media\\base.png";
	// current image is loaded
	Mat base_image = imread(name_base);
	string name_board = "C:\\Users\\User\\Desktop\\roomba\\media\\white.jpg";
	// current image is loaded
	Mat board_image = imread(name_board);

	//rectangle(room, Point(0, 0), Point(160,500),Scalar(0, 0, 0), -1, 8);

	//Keep an image with just the background (no robot painted on it)
	Mat collision_map = room.clone();



	//default testing picture
	Mat canard = imread("c:\\canard.jpg");
	imshow("canard", canard);


	//link rand seed to time to get a different rand effect each time the program is launched
	srand(time(NULL));

	Mat bg = extract_background(room);
	//create an image that will be a permanent update of the machine's representation of the appartment
	Mat memory_view = Mat::zeros(room.rows, room.cols, room.type());
	memory_view.setTo(cv::Scalar(255, 220, 220));
	

	Mat impact_view = Mat::zeros(room.rows, room.cols, room.type());
	impact_view.setTo(cv::Scalar(255, 255, 255));

	Mat memory_view_no_walls = memory_view.clone();
	
	Point roomba_coord;
	roomba_coord.x = board_image.rows / 2;// 36;
	roomba_coord.y = board_image.cols / 2;// 40;

	Point max_roomba_coord;
	max_roomba_coord.x = roomba_coord.x;

	double bg_height = 200;//bg.height-roomba.y
	//double angle = 91;


	/*for (double angle = 0; angle <= 90; angle++)
	{
		double adjacent = bg_height- roomba_coord.y;

		double hypotenuse = adjacent / cos(angle * PI / 180.0);

		double opposite = sqrt((hypotenuse * hypotenuse) - (adjacent * adjacent));

		int trunk_opposite = (int)opposite;

		max_roomba_coord.y = roomba_coord.x + trunk_opposite;

		cout << "degres : " << angle << " long. hypotenuse : " << hypotenuse << " long. opposite : " << opposite << " T " << trunk_opposite << " Y: " << max_roomba_coord.y << endl;

	}*/

	//double angle = 85;
		/*for (double angle = 0; angle < 90; angle = angle + 1)
		{
			int h = board_image.rows;
			int l = board_image.cols;
			for (double adjacent = 0; adjacent <= board_image.rows; adjacent++)
			{
				double hypotenuse = adjacent / cos(angle * PI / 180.0);
				double opposite = sqrt((hypotenuse * hypotenuse) - (adjacent * adjacent));
				int trunk_opposite = (int)opposite;
				max_roomba_coord.x = roomba_coord.x + adjacent;
				max_roomba_coord.y = roomba_coord.y + trunk_opposite;
				//cout << "adjacent : " << adjacent << " X: " << max_roomba_coord.x << " Y: " << max_roomba_coord.y << endl;
				if (max_roomba_coord.x < board_image.rows && max_roomba_coord.y < board_image.cols && max_roomba_coord.x >= 0 && max_roomba_coord.y >= 0)
					board_image.at<Vec3b>(max_roomba_coord.x, max_roomba_coord.y)[1] = 0;
			}
		}

		for (double angle = 0; angle < 90; angle = angle + 1)
		{
			int h = board_image.rows;
			int l = board_image.cols;
			for (double adjacent = 0; adjacent <= board_image.cols; adjacent++)
			{
				double hypotenuse = adjacent / cos(angle * PI / 180.0);
				double opposite = sqrt((hypotenuse * hypotenuse) - (adjacent * adjacent));
				int trunk_opposite = (int)opposite;
				max_roomba_coord.y = roomba_coord.y + adjacent;
				max_roomba_coord.x = roomba_coord.x - trunk_opposite;
				//cout << "adjacent : " << adjacent << " X: " << max_roomba_coord.x << " Y: " << max_roomba_coord.y << endl;
				if (max_roomba_coord.x < board_image.rows && max_roomba_coord.y < board_image.cols && max_roomba_coord.x >= 0 && max_roomba_coord.y >= 0)
					board_image.at<Vec3b>(max_roomba_coord.x, max_roomba_coord.y)[2] = 0;
			}
		}*/

	for (int quart = 1; quart <= 4; quart++)
	{
		for (double angle = 0; angle < 90; angle = angle + 1)
		{
			int h = board_image.rows;
			int l = board_image.cols;
			for (double adjacent = 0; adjacent <= board_image.rows; adjacent++)
			{
				double hypotenuse = adjacent / cos(angle * PI / 180.0);
				double opposite = sqrt((hypotenuse * hypotenuse) - (adjacent * adjacent));
				int trunk_opposite = (int)opposite;
				switch (quart)
				{
				case 1 :
					max_roomba_coord.x = roomba_coord.x + adjacent;
					max_roomba_coord.y = roomba_coord.y + trunk_opposite;
					break;
				case 2:
					max_roomba_coord.x = roomba_coord.x - adjacent;
					max_roomba_coord.y = roomba_coord.y + trunk_opposite;
					break;
				case 3:
					max_roomba_coord.x = roomba_coord.x - adjacent;
					max_roomba_coord.y = roomba_coord.y - trunk_opposite;
					break;
				case 4:
					max_roomba_coord.x = roomba_coord.x + adjacent;
					max_roomba_coord.y = roomba_coord.y - trunk_opposite;
					break;
				}
				/*max_roomba_coord.x = roomba_coord.x + adjacent;
				max_roomba_coord.y = roomba_coord.y - trunk_opposite;*/
				//cout << "adjacent : " << adjacent << " X: " << max_roomba_coord.x << " Y: " << max_roomba_coord.y << endl;
				if (max_roomba_coord.x < board_image.rows && max_roomba_coord.y < board_image.cols && max_roomba_coord.x >= 0 && max_roomba_coord.y >= 0)
				{
					switch (quart)
					{
					case 1 :
						board_image.at<Vec3b>(max_roomba_coord.x, max_roomba_coord.y)[1] = 0;
						break;
					case 2:
						board_image.at<Vec3b>(max_roomba_coord.x, max_roomba_coord.y)[2] = 0;
						break;
					case 3:
						board_image.at<Vec3b>(max_roomba_coord.x, max_roomba_coord.y)[0] = 0;
						break;
					case 4:
						board_image.at<Vec3b>(max_roomba_coord.x, max_roomba_coord.y)[1] = 0;
						board_image.at<Vec3b>(max_roomba_coord.x, max_roomba_coord.y)[0] = 0;
						break;
					}
				}
					//board_image.at<Vec3b>(max_roomba_coord.x, max_roomba_coord.y)[1] = 0;
			}
		}
	}

	
	// angle 0 - 359 degrees
	//for (double angle = 0; angle <= 359; angle = angle + 0.1)	
		int quart;
		int angle = rand() % 360;
		if (angle <= 89)
			quart = 1;
		else
			if (angle <= 179)
				quart = 2;
			else
				if (angle <= 269)
					quart = 3;
				else
					quart = 4;

		int h = board_image.rows;
		int l = board_image.cols;
		for (double adjacent = 0; adjacent <= board_image.rows; adjacent++)
		{
			double hypotenuse = adjacent / cos(angle * PI / 180.0);
			double opposite = sqrt((hypotenuse * hypotenuse) - (adjacent * adjacent));
			int trunk_opposite = (int)opposite;
			switch (quart)
			{
			case 1:
				max_roomba_coord.x = roomba_coord.x + adjacent;
				max_roomba_coord.y = roomba_coord.y + trunk_opposite;
				break;
			case 2:
				max_roomba_coord.x = roomba_coord.x - adjacent;
				max_roomba_coord.y = roomba_coord.y + trunk_opposite;
				break;
			case 3:
				max_roomba_coord.x = roomba_coord.x - adjacent;
				max_roomba_coord.y = roomba_coord.y - trunk_opposite;
				break;
			case 4:
				max_roomba_coord.x = roomba_coord.x + adjacent;
				max_roomba_coord.y = roomba_coord.y - trunk_opposite;
				break;
			}
			//cout << "adjacent : " << adjacent << " X: " << max_roomba_coord.x << " Y: " << max_roomba_coord.y << endl;

			if (max_roomba_coord.x < board_image.rows && max_roomba_coord.y < board_image.cols && max_roomba_coord.x >= 0 && max_roomba_coord.y >= 0)
			{
				board_image.at<Vec3b>(max_roomba_coord.x, max_roomba_coord.y)[0] = 0;
				board_image.at<Vec3b>(max_roomba_coord.x, max_roomba_coord.y)[1] = 0;
				board_image.at<Vec3b>(max_roomba_coord.x, max_roomba_coord.y)[2] = 0;
			}
		}


		//imshow("courbe", board_image);
	
	//waitKey();


	//robot robot1(200,150,100);
	//robot robot1(300, 100, 10);
	robot robot1;
	vector<Rect> nodes;
	view_nodes(room, nodes, robot1);

	int current_point = 1;
	Rect starting_coords = robot1.get_current_Coords();
	double chosen_angle = rand() % 360;
	Point next_direction;
	next_direction.x = -1;
	next_direction.y = -1;
	int timer = 0;
	int start_at = 0;
	int white_pixel_count = 0;
	while (1)
	{
		Mat background = room.clone();
		timer++;

		//obtain robot's default starting coords
		Rect robot_coords = robot1.get_current_Coords();

		//cout << "[" << robot_coords.x << "," << robot_coords.y << "] ";

		vector<Point> robot_path = show_direction(background, starting_coords, chosen_angle);

		/*int point_number = robot_path.size() - 1;
		point_number = (int) ((((float)robot_path.size()) / 100)*95);
		line(background, Point(robot_path.at(0).y, robot_path.at(0).x), Point(robot_path.at(point_number).y, robot_path.at(point_number).x), Scalar(0, 255, 0),2);
		Point circle2;
		circle2.x = robot_path.at(point_number).y;
		circle2.y = robot_path.at(point_number).x;
		circle(background, circle2, 6, Scalar(0, 255, 0), -1);*/

		/*system("cls");
		for (int i = 0; i < robot_path.size(); i++)
		{
			cout << "[" << robot_path[i].x << "," << robot_path[i].y << "] ";

		}*/

		int iiuii = 11;

		//vector out of range check
		try
		{
			if (current_point <= robot_path.size() && robot_path.size() > 0)
			{
				if (robot_coords.x == robot_path.at(current_point).y && robot_coords.x == robot_path.at(current_point).y)
					current_point++;
				if (robot_coords.x < robot_path.at(current_point).y)
				{
					robot_coords.x++;
					next_direction.x = 1;
				}
				if (robot_coords.x > robot_path.at(current_point).y)
				{
					robot_coords.x--;
					next_direction.x = -1;
				}
				if (robot_coords.y < robot_path.at(current_point).x)
				{
					robot_coords.y++;
					next_direction.y = 1;
				}
				//if (robot_coords.y > robot_path[current_point].x)
				if (robot_coords.y > robot_path.at(current_point).x)
				{
					robot_coords.y--;
					next_direction.y = -1;
				}
			}
		}
		catch (const std::out_of_range& oor) {
			std::cerr << "Out of Range error: " << oor.what() << '\n' << "current_point->" << current_point << endl;
		}



		//set to next step's coordinates
		/*robot_coords.x = robot_coords.x + next_direction.x;
		robot_coords.y = robot_coords.y + next_direction.y;*/


		//draw a rectangle where the robot should be (temp)
		//rectangle(background, Point(robot_coords.x, robot_coords.y), Point(robot_coords.x + robot_coords.width, robot_coords.y + robot_coords.height),
			//Scalar(0, 0, 0), -1, 8);
		int Xresize = robot_coords.width;//218
		int Yresize = robot_coords.height;//195
		resize(robot_image, robot_image, Size(Xresize, Yresize));


		//cout << endl;


		// copy robot image on the background	

		//cut background under the robot area (next position : is this going into a wall ?)
		Mat subImg = collision_map(cv::Range(robot_coords.y, robot_coords.y + robot_coords.height),
			cv::Range(robot_coords.x, robot_coords.x + robot_coords.width));

		if (check_collision(subImg) == true)//no collision
		{



			robot1.change_current_Coords(next_direction.x, next_direction.y);


		}
		else//collision
		{
			/*next_direction.x = -next_direction.x;
			next_direction.y = -next_direction.y;*/
			current_point = 1;
			starting_coords = robot1.get_current_Coords();
			//keep previous angle for visual rotation effect
			double old_angle = chosen_angle;
			//new angle
			//old version : choose a totally random angle
			//chosen_angle = rand() % 360;
			//new version : choose a random angle at the opposite direction of incoming direction(in the 180 degrees behind the robot)
			chosen_angle = old_angle + 90 + (rand() % 180);
			if (chosen_angle > 360)
				chosen_angle = chosen_angle - 360;

			double rota = old_angle;
			//for (int rota = old_angle; rota < 360; rota++)
			bool show_rotation_after_collision = false;
			while (show_rotation_after_collision)
			{

				if (rota < chosen_angle)
					rota++;
				if (rota > chosen_angle)
					rota--;

				Rect updated_coords = robot_coords;
				Mat robot_image_clone = robot_image.clone();
				Point2f src_center(robot_image_clone.cols / 2.0F, robot_image_clone.rows / 2.0F);
				Mat rot_mat = getRotationMatrix2D(src_center, rota, 1.0);
				Mat dst;
				warpAffine(robot_image_clone, dst, rot_mat, robot_image_clone.size(),
					INTER_LINEAR, BORDER_CONSTANT, Scalar(255, 255, 255));

				Mat mask = dst.clone();
				cvtColor(mask, mask, CV_BGR2GRAY);
				threshold(mask, mask, 200, 255, THRESH_BINARY_INV);
				dst.copyTo(background(Rect(updated_coords.x, updated_coords.y, dst.cols, dst.rows)), mask);


				imshow("mask", mask);
				imshow("room", background);
				waitKey(1);


				//beware,this will bug with fragment values like 0.5
				if (rota == chosen_angle)
					break;

			}
		}

		//base_image.copyTo(background(Rect(550, 150, base_image.cols, base_image.rows)));
		Rect updated_coords = robot_coords;// robot1.get_current_Coords();
		//show_direction(background, updated_coords, chosen_angle);



		Mat robot_image_clone = robot_image.clone();
		Point2f src_center(robot_image_clone.cols / 2.0F, robot_image_clone.rows / 2.0F);
		Mat rot_mat = getRotationMatrix2D(src_center, chosen_angle, 1.0);
		Mat dst;
		warpAffine(robot_image_clone, dst, rot_mat, robot_image_clone.size(),
			INTER_LINEAR, BORDER_CONSTANT, Scalar(255, 255, 255));



		Mat mask = dst.clone();
		cvtColor(mask, mask, CV_BGR2GRAY);
		threshold(mask, mask, 200, 255, THRESH_BINARY_INV);
		//imshow("mask", mask);
		dst.copyTo(background(Rect(updated_coords.x, updated_coords.y, dst.cols, dst.rows)), mask);




		print_value(updated_coords.x, background, 420, 420);
		print_value(updated_coords.y, background, 420, 480);



		//draw walls in the memory view when collision (warning, here it's general loop)
		Mat frag_mask = subImg.clone();
		threshold(frag_mask, frag_mask, 200, 255, THRESH_BINARY_INV);
		subImg.copyTo(memory_view(Rect(updated_coords.x, updated_coords.y, robot_image.cols, robot_image.rows)), frag_mask);

		//draw white where the robot just cleaned
		Point circleCenter;
		circleCenter.x = updated_coords.x + robot1.get_robot_size() / 2;
		circleCenter.y = updated_coords.y + robot1.get_robot_size() / 2;
		circle(bg, circleCenter, robot1.get_robot_size() / 2, Scalar(255, 255, 255), -1);

		circle(memory_view, circleCenter, robot1.get_robot_size() / 2, Scalar(255, 255, 255), -1);


		circle(memory_view_no_walls, circleCenter, robot1.get_robot_size() / 2, Scalar(255, 255, 255), -1);



		bool search_unknown_areas = true;
		subImg.copyTo(impact_view(Rect(updated_coords.x, updated_coords.y, robot_image.cols, robot_image.rows)), frag_mask);
		subImg.copyTo(impact_view(Rect(updated_coords.x + 1, updated_coords.y, robot_image.cols, robot_image.rows)), frag_mask);
		subImg.copyTo(impact_view(Rect(updated_coords.x, updated_coords.y + 1, robot_image.cols, robot_image.rows)), frag_mask);
		vector<vector<Point>> contours2;
		vector<Vec4i> hierarchy2;
		if (search_unknown_areas == true && timer % 100 == 0)
		{
			Mat ground_contours_pic = memory_view_no_walls.clone();
			cvtColor(ground_contours_pic, ground_contours_pic, CV_BGR2GRAY);
			threshold(ground_contours_pic, ground_contours_pic, 254, 255, THRESH_BINARY);
			vector<vector<Point>> contours;
			vector<Vec4i> hierarchy;
			findContours(ground_contours_pic, contours, hierarchy, CV_RETR_TREE, CHAIN_APPROX_SIMPLE);
			//subImg.copyTo(memory_view_no_walls(Rect(updated_coords.x, updated_coords.y, robot_image.cols, robot_image.rows)), frag_mask);
			Mat canvas = Mat::zeros(memory_view_no_walls.rows, memory_view_no_walls.cols, memory_view_no_walls.type());
			Mat show_conts = memory_view_no_walls.clone();
			Scalar colour(255, 255, 255);
			for (int i = 0; i < contours.size(); i++)
			{
				//draw all raw contours found on
				drawContours(canvas, contours, i, colour, 1, 1, hierarchy, 0, Point());
			}
			//imshow("subImg", subImg);
			Mat reverse_impact = impact_view.clone();
			bitwise_not(reverse_impact, reverse_impact);
			Mat common;
			bitwise_and(canvas, reverse_impact, common);
			Mat xor_map;
			bitwise_xor(canvas, common, xor_map);
			//imshow("conts", canvas);
			//imshow("impacts", reverse_impact);
			//imshow("common", common);
			threshold(xor_map, xor_map, 200, 255, THRESH_BINARY);
			//imshow("xormap", xor_map);
			Mat memory_view_clone = memory_view.clone();
			

			cvtColor(xor_map, xor_map, CV_BGR2GRAY);
			findContours(xor_map, contours2, hierarchy2, CV_RETR_TREE, CHAIN_APPROX_SIMPLE);
			for (int i = 0; i < contours2.size(); i++)
			{
				//cout<< contours2[i].size()<<" ";
				Scalar colour(0, 255, 0);
				//draw all raw contours found on
				if (contours2[i].size() > 30)
					drawContours(memory_view_clone, contours2, i, colour, 1, 1, hierarchy2, 0, Point());
			}
			imshow("refconts", memory_view_clone);
		}

		
		//---------------------------------------------------------------------------------------
		//nodes
		

		//generate random position
		/*Mat node_view = room.clone();
		imshow("nodes", node_view);


		int rand_x = rand() % (node_view.cols - robot_coords.width);
		int rand_y = rand() % (node_view.rows - robot_coords.height);

		
		Rect area_of_interest;
		area_of_interest.x = rand_x;
		area_of_interest.y = rand_y;
		area_of_interest.height = robot1.get_robot_size();
		area_of_interest.width = robot1.get_robot_size();
		Mat frag_check = room(area_of_interest);

		imshow("frag check", frag_check);

		Mat white_area = Mat::zeros(frag_check.rows, frag_check.cols, frag_check.type());
		white_area.setTo(cv::Scalar(255, 255, 255));

		Mat diff;
		cvtColor(white_area, white_area, CV_BGR2GRAY);
		cvtColor(frag_check, frag_check, CV_BGR2GRAY);
		compare(white_area, frag_check, diff, cv::CMP_NE);
		int nz = countNonZero(diff);

		

		//Scalar intensity = room.at<Vec3b>(rand_y, rand_x);
		if (nz == 0)//intensity.val[1] == 255 && nz == 0)
		{

			Rect node_area;
			node_area.x = rand_x;
			node_area.y = rand_y;
			node_area.height = robot1.get_robot_size();
			node_area.width = robot1.get_robot_size();
			rectangle(room, node_area, Scalar(0, 0, 0), 1, 8);

			Point node_Center;
			node_Center.x = rand_x + (robot1.get_robot_size() / 2);
			node_Center.y = rand_y + (robot1.get_robot_size() / 2);
			circle(room, node_Center, 3, Scalar(0, 0, 255), -1);

			nodes.push_back(node_area);
		}

		if (nodes.size() > 1)//2 nodes min
			for (int current_node = 0; current_node < nodes.size(); current_node++)
			{
				for (int other_node = current_node; other_node < nodes.size(); other_node++)
				{
					if (current_node != other_node)
					{
						line(room, Point(nodes[current_node].x + (robot1.get_robot_size() / 2), nodes[current_node].y + (robot1.get_robot_size() / 2)), Point(nodes[other_node].x + (robot1.get_robot_size() / 2), nodes[other_node].y + (robot1.get_robot_size() / 2)), Scalar(255,0,0), 1, 8);

					}
				}
			}*/
		//-----------------------------------------------------------------------------------


		//do a white pixel count in the memory view image, corresponds to total surface found.
		//this should be altered later to be run only from time to time, as it's a costly operation.
		if (timer % 214 == 0)
		{
			int temp_pix_counter = 0;
			for (int row = 0; row < memory_view.rows; row++)
			{
				for (int col = 0; col < memory_view.cols; col++)
				{
					Scalar intensity = memory_view.at<Vec3b>(row, col);
					if (intensity.val[1] == 255)
						temp_pix_counter++;
				}
			}
			white_pixel_count = temp_pix_counter;
		}

		if (white_pixel_count > 0)
			int fff = 1;
		print_value((float)white_pixel_count /3000, background, 100, 420);

		

		robot1.change_battery();
		int g = 0; int r = 0;
		if (robot1.get_battery() > 30)
		{
			g = 180; r =  0;
		}
		else
		{
			g = 0; r = 180;
		}
		rectangle(background, Point(10, 12), Point(25, 30), Scalar(0, g, r), -1, 8);
		print_value(robot1.get_battery(), background, 20, 40);


		//finally we resize the result pictures so all of them could fit in the screen
		bool shrink = false;
		if (shrink == true)
		{
			int X_resize = 400;
			int Y_resize = 300;
			Mat bg_visual = bg.clone();
			Mat background_visual = background.clone();
			resize(bg, bg_visual, Size(X_resize, Y_resize));
			resize(background, background_visual, Size(X_resize, Y_resize));
			imshow("bg", bg_visual);
			imshow("room", background_visual);
			imshow("fragment", subImg);
			waitKey(1);
		}
		else
		{
			if (timer > start_at)
			{
				imshow("bg", bg);
				
				//imshow("fragment", subImg); 
				if(contours2.size() > 0)
					for (int i = 0; i < contours2.size(); i++)
					{
						//cout<< contours2[i].size()<<" ";
						Scalar colour(0, 255, 0);
						//draw all raw contours found on
						if (contours2[i].size() > 30)
							drawContours(memory_view, contours2, i, colour, 1, 1, hierarchy2, 0, Point());
					}
				imshow("machine memory representation of the room", memory_view);
				//imshow("machine memory, just the room", ground_contours_pic);
				//imshow("ground contours", show_conts);
				imshow("room", background);
				waitKey(1);
			}
		}

		

	}

	return(0);
}