#include <iostream>
#include <cmath>
#include <vector>

#include <vtkDoubleArray.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkXMLStructuredGridWriter.h>
#include <vtkStructuredGrid.h>
#include <vtkSmartPointer.h>

using namespace std;

class CalcNode
{
friend class CalcMesh;

protected:
    double x;
    double y;
    double intensity;
    double Eo;
    double E;
    double lambda;
    double beta;

public:
    // Конструктор по умолчанию
    CalcNode() : x(0.0), y(0.0), intensity(0.0), E(0.0)
    {
    }

    // Конструктор с указанием всех параметров
    CalcNode(double x, double y, double intensity, double Eo, double lambda, double beta)
            : x(x), y(y), intensity(intensity), Eo(Eo), lambda(lambda), beta(beta)
    {
    }
    void move(double t) {
        E = 2*Eo*sin(2*M_PI*3e+10/(lambda * 1e-7)*t)*cos(2*M_PI*x*cos(beta)/(lambda * 1e-7));
    }
};

// Класс расчётной сетки
class CalcMesh
{
protected:
    // 2D-сетка из расчётных точек
    vector<vector<CalcNode>> points;

public:
    // Конструктор сетки size x size точек с шагом h по пространству
    CalcMesh(unsigned int size, double h, double Eo, double alpha, double lambda) {
        points.resize(size);
        for(unsigned int i = 0; i < size; i++) {
            points[i].resize(size);
            for(unsigned int j = 0; j < size; j++) {
                // Начальные координаты зададим равномерно в плоскости OXY
                double beta = (90 - alpha/2)/180*M_PI;
                double pointX = i * h;
                double pointY = j * h;
                double intesity = 2*Eo*Eo*(1 + cos(4*M_PI * sin(alpha*M_PI/180/2) / (lambda * 1e-7) * pointX));

                points[i][j] = CalcNode(pointX, pointY, intesity, Eo, lambda, beta);
            }
        }
    }
    void doTimeSteps(double tau, int N) {
        // По сути метод просто двигает все точки
        double t = 0;
        for(unsigned int step = 0; step < N; step++) {
            t=tau*step;
            for(unsigned int i = 0; i < points.size(); i++) {
                for(unsigned int j = 0; j < points[i].size(); j++) {
                    points[i][j].move(t);
                }
            }
            this->snapshot(step);
        }


    }


    // Метод отвечает за запись текущего состояния сетки в снапшот в формате VTK
    void snapshot(unsigned int snap_number) {
        // Сетка в терминах VTK
        vtkSmartPointer<vtkStructuredGrid> structuredGrid = vtkSmartPointer<vtkStructuredGrid>::New();
        // Точки сетки в терминах VTK
        vtkSmartPointer<vtkPoints> dumpPoints = vtkSmartPointer<vtkPoints>::New();

        // Скалярное поле на точках сетки
        auto intensity = vtkSmartPointer<vtkDoubleArray>::New();
        intensity->SetName("intensity");
        auto E = vtkSmartPointer<vtkDoubleArray>::New();
        E->SetName("E");


        // Обходим все точки нашей расчётной сетки
        unsigned int number = (unsigned int)points.size();
        for(unsigned int i = 0; i < number; i++) {
            for(unsigned int j = 0; j < number; j++) {
                // Вставляем новую точку в сетку VTK-снапшота
                dumpPoints->InsertNextPoint(points[i][j].x, points[i][j].y, 0);

                // И значение скалярного поля тоже
                intensity->InsertNextValue(points[i][j].intensity);
                E->InsertNextValue(points[i][j].E);

            }
        }

        // Задаём размеры VTK-сетки (в точках, по трём осям)
        structuredGrid->SetDimensions(number, number, 1);
        // Грузим точки в сетку
        structuredGrid->SetPoints(dumpPoints);

        // Присоединяем векторное и скалярное поля к точкам
        structuredGrid->GetPointData()->AddArray(intensity);
        structuredGrid->GetPointData()->AddArray(E);

        // Создаём снапшот в файле с заданным именем
        string fileName = "inter_E_I" + std::to_string(snap_number) + ".vts";
        vtkSmartPointer<vtkXMLStructuredGridWriter> writer = vtkSmartPointer<vtkXMLStructuredGridWriter>::New();
        writer->SetFileName(fileName.c_str());
        writer->SetInputData(structuredGrid);
        writer->Write();
    }
};

int main()
{
    unsigned int size = 100;
    double tau = 0.1;
    int N = 100;
    double h = 1e-5; // см
    double Eo = 1; // В/см
    double alpha = 60;
    double lambda = 500; // нм
    CalcMesh mesh(size, h, Eo, alpha, lambda);
    mesh.doTimeSteps(tau, N);


    return 0;
}