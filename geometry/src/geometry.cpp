#include <cmath>
#include <vector>
#include "geometry.h"

#define PI 3.14159265

using std::vector;

double EPS = 2E-7;

Point::Point(double x, double y) {
  this->x = x;
  this->y = y;
}

void Point::rotate(Point center, double angle) {
  double new_x =
    (x - center.x) * cos(angle * PI / 180.0)
    - (y - center.y) * sin(angle * PI / 180.0)
    + center.x;

  double new_y =
    (x - center.x) * sin(angle * PI / 180.0)
    + (y - center.y) * cos(angle * PI / 180.0)
    + center.y;

  x = new_x;
  y = new_y;
}

void Point::scale(Point center, double coefficient) {
    x = (x - center.x) * coefficient;
    y = (y - center.y) * coefficient;
}

bool Point::operator==(const Point& point) const {
  return fabs(x - point.x) < EPS && fabs(y - point.y) < EPS;
}

bool Point::operator!=(const Point& point) const {
  return !(*this == point);
}

Line::Line(Point a, Point b) {
  if (a == b) {
    throw "Points should be different.";
  }

  k = (b.y - a.y) / (b.x - a.x);
  this->b = (b.x * a.y - a.x * b.y) / (b.x - a.x);
}

Line::Line(double k, double b) {
  this->k = k;
  this->b = b;
}

Line::Line(Point a, double k) {
  this->k = k;
  b = a.y - k * a.x;
}

void Line::rotate(Point center, double angle) {
  double x1 = rand();
  double x2 = rand();
  double y1 = k * x1 + b;
  double y2 = k * x2 + b;

  Point p1(x1, y1);
  Point p2(x2, y2);

  p1.rotate(center, angle);
  p2.rotate(center, angle);

  k = (p2.y - p1.y) / (p2.x - p1.x);
  b = (p2.x * p1.y - p1.x * p2.y) / (p2.x - p1.x);
}

bool Line::operator==(const Line& line) const {
  return fabs(k - line.k) < EPS && fabs(b - line.b) < EPS;
}

bool Line::operator!=(const Line& line) const {
  return !(*this == line);
}

void Shape::rotate(Point center, double angle) {
  for (auto& point : getPoints()) {
    point.rotate(center, angle);
  }
}

void Shape::reflex(Point center) {
  rotate(center, 180);
}

void Shape::reflex(Line axis) {
  double k, b, x, y;

  k = - 1 / axis.k;

  for (auto& point : getPoints()) {
    b = point.y - k * point.x;

    x = (b - axis.b) / (axis.k - k);
    y = axis.k * x + axis.b;
    point.rotate(Point(x, y), 180);
  }
}

void Shape::scale(Point center, double coefficient) {
  for (auto& point : getPoints()) {
    point.scale(center, coefficient);
  }
}

Polygon::Polygon() {}

Polygon::Polygon(vector<Point> vertices) : vertices(vertices) {}

void Polygon::setVertices(vector<Point> vertices) {
  this->vertices.empty();
  this->vertices = vertices;
}

vector<Point>& Polygon::getPoints() {
  return vertices;
}

int Polygon::verticesCount() const {
  return vertices.size();
}

const vector<Point> Polygon::getVertices() const {
  return vertices;
}

double Polygon::perimeter() const {
  double result = 0;
  int size = vertices.size();

  for (int i = 1; i < size; ++i) {
    result += sqrt(pow(vertices[i].x - vertices[i - 1].x, 2) + pow(vertices[i].y - vertices[i - 1].y, 2));
  }

  result += sqrt(pow(vertices[0].x - vertices[size - 1].x, 2) + pow(vertices[0].y - vertices[size - 1].y, 2));

  return result;
}

double Polygon::area() const {
  double result = 0;
  int size = vertices.size();

  for (int i = 1; i < size; ++i) {
    result += (vertices[i - 1].x * vertices[i].y - vertices[i].x * vertices[i - 1].y) / 2;
  }

  result += (vertices[size - 1].x * vertices[0].y - vertices[0].x * vertices[size - 1].y) / 2;

  return fabs(result);
}

bool Polygon::operator==(const Shape& shape) const {
  const Polygon* polygon = dynamic_cast<const Polygon*>(&shape);

  return polygon != NULL
    && perimeter() == polygon->perimeter()
    && area() == polygon->area();
}

bool Polygon::operator!=(const Shape& shape) const {
  return !(*this == shape);
}

Ellipse::Ellipse(Point f1, Point f2, double sum) : points({f1, f2}) {
  a = sum / 2;
  double square_distance = (pow(f2.x - f1.x, 2) + pow(f2.y - f1.y, 2)) / 4;
  b = sqrt(a * a - square_distance);
}

vector<Point>& Ellipse::getPoints() {
  return points;
}

const std::pair<Point, Point> Ellipse::focuses() const {
  return std::make_pair(points[0], points[1]);
}

double Ellipse::eccentricity() const {
  double c = sqrt(a * a - b * b);
  return c / a;
}

double Ellipse::perimeter() const {
  return PI * (3 * (a + b) - sqrt((3 * a + b) * (a + 3 * b)));
}

double Ellipse::area() const {
  return PI * a * b;
}

void Ellipse::scale(Point center, double coefficient) {
  Shape::scale(center, coefficient);
  a = fabs(a * coefficient);
  b = fabs(b * coefficient);
}

bool Ellipse::operator==(const Shape& shape) const {
  const Ellipse* ellipse = dynamic_cast<const Ellipse*>(&shape);

  return ellipse != NULL
    && (perimeter() - ellipse->perimeter()) < EPS
    && (area() - ellipse->area()) < EPS;
}

bool Ellipse::operator!=(const Shape& shape) const {
  return !(*this == shape);
}

Circle::Circle(Point center, double r) : Ellipse(center, center, 2 * r), points({center}) {
  this->r = r;
}

vector<Point>& Circle::getPoints() {
  return points;
}

double Circle::radius() const {
  return r;
}

const Point Circle::center() const {
  return points[0];
}

void Circle::scale(Point center, double coefficient) {
  Ellipse::scale(center, coefficient);
  r = fabs(r * coefficient);
}

bool Circle::operator==(const Shape& shape) const {
  const Circle* circle = dynamic_cast<const Circle*>(&shape);

  return circle != NULL
    && (perimeter() - circle->perimeter()) < EPS
    && (area() - circle->area()) < EPS;
}

bool Circle::operator!=(const Shape& shape) const {
  return !(*this == shape);
}

Rectangle::Rectangle(Point leftTop, Point rightBottom, double ratio)
  : c(Point((leftTop.x + rightBottom.x) / 2, (leftTop.y + rightBottom.y) / 2)) {
    double square_cos = 1 / (ratio * ratio + 1);
    double square_sin = 1 - square_cos;

    double a = sqrt((pow(leftTop.x - rightBottom.x, 2) + pow(leftTop.y - rightBottom.y, 2)) / (1 - ratio));
    double b = ratio * a;

    double angle = atan(ratio) * 180 / PI;
    Point leftBottom(leftTop.x, leftTop.y);
    leftBottom.rotate(rightBottom, angle);
    double coefficient = sqrt(pow(leftBottom.x - rightBottom.x, 2) + pow(leftBottom.y - rightBottom.y, 2)) / a;
    leftBottom.x = (leftBottom.x - rightBottom.x) * coefficient + rightBottom.x;
    leftBottom.y = (leftBottom.y - rightBottom.y) * coefficient + rightBottom.y;

    Point rightTop(leftBottom.x, leftBottom.y);
    rightTop.rotate(c, 180);
    setVertices({leftTop, rightTop, rightBottom, leftBottom});
}

const Point Rectangle::center() const {
  return c;
}

const std::pair<Line, Line> Rectangle::diagonals() const {
  auto points = getVertices();
  return std::make_pair(Line(points[0], points[2]), Line(points[1], points[3]));
}

bool Rectangle::operator==(const Shape& shape) const {
  const Rectangle* rectangle = dynamic_cast<const Rectangle*>(&shape);

  if (rectangle != NULL) {
    return fabs(perimeter() - rectangle->perimeter()) < EPS && fabs(area() - rectangle->area()) < EPS;
  }

  const Square* square = dynamic_cast<const Square*>(&shape);

  return square != NULL
    && fabs(perimeter() - square->perimeter()) < EPS
    && fabs(area() - square->area()) < EPS;
}

bool Rectangle::operator!=(const Shape& shape) const {
  return !(*this == shape);
}

Square::Square(Point leftTop, Point rightBottom) : Rectangle(leftTop, rightBottom, 1) {}

Circle Square::circumscribedCircle() const {
  auto c = center();
  auto points = getVertices();
  double radius = sqrt(pow(points[0].x - c.x, 2) + pow(points[0].y - c.y, 2));
  return Circle(Point(c.x, c.y), radius);
}

Circle Square::inscribedCircle() const {
  auto c = center();
  auto points = getVertices();
  double radius = sqrt(pow(points[0].x - points[1].x, 2) + pow(points[0].y - points[1].y, 2)) / 2;
  return Circle(Point(c.x, c.y), radius);
}

bool Square::operator==(const Shape& shape) const {
  const Square* square = dynamic_cast<const Square*>(&shape);

  if (square != NULL) {
    return fabs(perimeter() - square->perimeter()) < EPS
    && fabs(area() - square->area()) < EPS;
  }

  const Rectangle* rectangle = dynamic_cast<const Rectangle*>(&shape);

  return rectangle != NULL
    && fabs(perimeter() - rectangle->perimeter()) < EPS
    && fabs(area() - rectangle->area()) < EPS;
}

bool Square::operator!=(const Shape& shape) const {
  return !(*this == shape);
}

Triangle::Triangle(Point a, Point b, Point c) : Polygon({a, b, c}) { }

double Triangle::area() const {
  auto points = getVertices();
  double a = sqrt(pow(points[0].x - points[1].x, 2) + pow(points[0].y - points[1].y, 2));
  double b = sqrt(pow(points[1].x - points[2].x, 2) + pow(points[1].y - points[2].y, 2));
  double c = sqrt(pow(points[2].x - points[0].x, 2) + pow(points[2].y - points[0].y, 2));

  double p = (a + b + c) / 2;
  return sqrt(p * (p - a) * (p - b) * (p - c));
}

Circle Triangle::circumscribedCircle() const {
  auto points = getVertices();
  Line a(points[0], points[1]);
  Line b(points[1], points[2]);
  Point c1 = Point((points[0].x + points[1].x) / 2, (points[0].y + points[1].y) / 2);
  Point c2 = Point((points[1].x + points[2].x) / 2, (points[1].y + points[2].y) / 2);
  a.rotate(c1, 90);
  b.rotate(c2, 90);

  double x = (b.b - a.b) / (a.k - b.k);
  double y = a.k * x + a.b;

  double radius = sqrt(pow(points[0].x - x, 2) + pow(points[0].y - y, 2));
  return Circle(Point(x, y), radius);
}

Circle Triangle::inscribedCircle() const {
  double radius = 2 * area() / perimeter();
  auto points = getVertices();
  double a = sqrt(pow(points[0].x - points[1].x, 2) + pow(points[0].y - points[1].y, 2));
  double b = sqrt(pow(points[1].x - points[2].x, 2) + pow(points[1].y - points[2].y, 2));
  double c = sqrt(pow(points[2].x - points[0].x, 2) + pow(points[2].y - points[0].y, 2));
  double alpha = acos((b * b + c * c - a * a) / (2 * b * c)) * 180 / PI;
  double betta = acos((a * a + c * c - b * b) / (2 * a * c)) * 180 / PI;

  Line bis1(points[0], points[2]);
  Line bis2(points[1], points[2]);
  bis1.rotate(points[0], -betta / 2);
  bis2.rotate(points[2], -alpha / 2);

  double x = (bis2.b - bis1.b) / (bis1.k - bis2.k);
  double y = bis1.k * x + bis1.b;

  return Circle(Point(x, y), radius);
}

Point Triangle::centroid() const {
  auto points = getVertices();
  Line med1(points[0], Point((points[1].x + points[2].x) / 2, (points[1].y + points[2].y) / 2));
  Line med2(points[1], Point((points[0].x + points[2].x) / 2, (points[0].y + points[2].y) / 2));

  double x = (med2.b - med1.b) / (med1.k - med2.k);
  double y = med1.k * x + med1.b;

  return Point(x, y);
}

Point Triangle::orthocenter() const {
  auto points = getVertices();
  Line a(points[0], points[1]);
  Line b(points[0], points[2]);

  Point sim_a(points[2].x, points[2].y);
  Point sim_b(points[1].x, points[1].y);

  Polygon pol1({sim_a});
  pol1.reflex(a);
  Polygon pol2({sim_b});
  pol2.reflex(b);

  Line ver1(points[2], pol1.getVertices()[0]);
  Line ver2(points[1], pol2.getVertices()[0]);

  double x = (ver2.b - ver1.b) / (ver1.k - ver2.k);
  double y = ver1.k * x + ver1.b;

  return Point(x, y);
}

Line Triangle::EulerLine() const {
  return Line(centroid(), orthocenter());
}

Circle Triangle::ninePointsCircle() const {
  Circle circumscribed = circumscribedCircle();
  Point orth = orthocenter();
  return Circle(Point((circumscribed.center().x + orth.x) / 2, (circumscribed.center().y + orth.y) / 2), circumscribed.radius() / 2);
}

bool Triangle::operator==(const Shape& shape) const {
  const Triangle* triangle = dynamic_cast<const Triangle*>(&shape);

  return triangle != NULL
    && perimeter() == triangle->perimeter()
    && area() == triangle->area();
}

bool Triangle::operator!=(const Shape& shape) const {
  return !(*this == shape);
}
