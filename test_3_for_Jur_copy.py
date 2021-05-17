import math as m
import numpy as np
import sys
import vtk
import random
# Класс расчётной сетки
class CalcMesh():

# Конструктор сетки size x size точек с шагом h по пространству
	def __init__(self, size):
		# 2D-сетка из расчётных точек, у каждой из которых, тем не менее, 3 координаты
		self.X = np.zeros((size, size))
		self.Y = np.zeros((size, size))
		self.eps = sys.float_info.epsilon
		self.r = 1
		self.R = 3
		self.size = 200
		self.dt = 1e-10
		self.h = 1/size
		self.circle = []
		for i in range(size-1):
			al = i*2*m.pi/(size-1)
			x = self.r*m.cos(al)
			y = self.r*m.sin(al)
			self.circle.append([x, y])
		self.circle.append(self.circle[0])
		#print(self.circle)
		self.curve = []
		for i in range(size-1):
			al = i*2*m.pi/(size-1)
			rho = self.R+random.randint(-1,1)/10
			#rho = self.R
			x = rho*m.cos(al)
			y = rho*m.sin(al)
			self.curve.append([x, y])
		self.curve.append(self.curve[0])
		#print(self.curve)

		#print(self.circle)
		#print('До преобразования\n')

		#print('После преобразоваения\n')

		#граничные условия для боковых сторон квадрата
		for i in range(self.size):
			self.X[i][size-1] = self.curve[i][0]
			self.Y[i][size-1] = self.curve[i][1]
			self.X[i][0] = self.circle[i][0]
			self.Y[i][0] = self.circle[i][1]
		#Далее для итерационного метода задаём какие-то ненулевые начальные координаты точек.
		#Для этого сначала проведем size радиальных линий, используя точки граничных условий и
		# на каждой радиальной линии равномерно распределим size точек. Нам нужно +- адекватное начальное приближение
		#иначе метод не сойдется
		self.k = [0]*(size)
		self.b = [0]*(size)

		for i in range(size):
			if(self.X[i][0] != self.X[i][size-1]):
				self.k[i] = (self.Y[i][0] - self.Y[i][size-1]) / (self.X[i][0] - self.X[i][size-1])
				self.b[i] = self.Y[i][0] - self.k[i] * self.X[i][0]
				a = np.linspace(self.X[i][0], self.X[i][size-1],num=size-1, endpoint=False)
				self.X[i][1:size-1] = a[1:]
				cons = a*self.k[i]+self.b[i]
				self.Y[i][1:size-1] = cons[1:]
			else:
				self.X[i][1:size-1] = self.X[i][0]
				a = np.linspace(self.Y[i][0], self.Y[i][size-1], num=size - 1, endpoint=False)
				self.Y[i][1:size-1] = a[1:]
		#if (self.circle[size-1][0] != self.curve[size-1][0]):
		#	self.k[size-1] = (self.circle[size-1][1] - self.curve[size-1][1]) / (self.circle[size-1][0] - self.curve[size-1][0])
		#	self.b[size-1] = self.circle[size-1][1] - self.k[size-1] * self.circle[size-1][0]
		#	a = np.linspace(self.circle[i][0], self.curve[i][0], num=size - 1, endpoint=False)
		#	self.X[i][1:size - 1] = a[size-1:0:-1]
		#	cons = a * self.k[i] + self.b[i]
		#	self.Y[i][1:size - 1] = cons[size-1:0:-1]
		#else:
		#	self.X[i][1:size - 1] = self.curve[i][0]
		#	a = np.linspace(self.circle[i][1], self.curve[i][1], num=size - 1, endpoint=False)
		#	self.Y[i][1:size - 1] = a[size-1:0:-1]
		#print('\nX\n', self.X, '\nY\n', self.Y)

		

	# Метод отвечает за запись текущего состояния сетки в снапшот в формате VTK
	def snapshot(self, snap_number):
		# Сетка в терминах VTK
		structuredGrid = vtk.vtkStructuredGrid()
		# Точки сетки в терминах VTK
		points = vtk.vtkPoints()

		# Обходим все точки нашей расчётной сетки
		# Делаем это максимально неэффективным, зато наглядным образом
		number = len(self.X)
		for i in range(0, number):
			for j in range(0, number):
				# Вставляем новую точку в сетку VTK-снапшота
				points.InsertNextPoint(self.X[i][j], self.Y[i][j], 0)

		# Задаём размеры VTK-сетки (в точках, по трём осям)
		structuredGrid.SetDimensions(number, number, 1)
		# Грузим точки в сетку
		structuredGrid.SetPoints(points)

		# Создаём снапшот в файле с заданным именем
		writer = vtk.vtkXMLStructuredGridWriter()
		writer.SetInputDataObject(structuredGrid)
		writer.SetFileName("grid_test_aaa2" + str(snap_number) + ".vts")
		writer.Write()
		
		
	def laplas(self,dt, h):
		self.val_previous = [1, 1]
		max_difference = 1.
		while (max_difference > self.eps):
			max_difference = 0.
			for i in range(1, self.size-1):
				for j in range(1, self.size-1):
					self.val_previous = np.array([self.X[i][j], self.Y[i][j]])
					fx_i_j = self.X[i][j]
					fx_1 = self.X[i+1][j]
					fx_2 = self.X[i-1][j]
					fx_3 = self.X[i][j-1]
					fx_4 = self.X[i][j+1]
					fy_i_j = self.Y[i][j]
					fy_1 = self.Y[i+1][j]
					fy_2 = self.Y[i-1][j]
					fy_3 = self.Y[i][j-1]
					fy_4 = self.Y[i][j+1]
					self.X[i][j] = fx_i_j - (dt/h**2) * (fx_1 + fx_2 + fx_4 + fx_3 - 4*fx_i_j)
					self.Y[i][j] = fy_i_j - (dt/h**2) * (fy_1 + fy_2 + fy_4 + fy_3 - 4*fy_i_j)
					length = m.sqrt((self.X[i][j] - self.val_previous[0])**2 + (self.Y[i][j] - self.val_previous[1])**2)
					if (length > max_difference):
						max_difference = length
			print(max_difference)

# Размер расчётной сетки, точек на сторону
size = 200

# Шаг по времени
tau = 1e-10

# Создаём сетку заданного размера
grid = CalcMesh(size)
#grid.laplas(tau,1/size)
# Пишем её начальное состояние в VTK
grid.snapshot(0)
