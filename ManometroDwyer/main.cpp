#include <iostream>
#include <fstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;


Mat PreproceCamara(){
	Mat imagen;
	cout<<"Abriendo Camara"<<endl;
    VideoCapture cap(0, cv::CAP_DSHOW);
	if (!cap.isOpened()) {
        cerr << "No se pudo abrir la cámara." << endl;
        
		}else{	
		cout << "Camara abierta" << endl;
		cap.read(imagen);
		return imagen;
	}
}
cv::Mat detectAndAlignRectangle(const cv::Mat& input) {
    
    cv::Mat gray, blurred, edges;
    cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);      // Convertir a escala de grises
    cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 1); 
    cv::Canny(blurred, edges, 50, 150);                
	
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(edges, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	
    // Paso 3: Filtrar contornos que parezcan rectángulos
    std::vector<cv::Point> rectangle;
    for (const auto& contour : contours) {
        double epsilon = 0.02 * cv::arcLength(contour, true); // Tolerancia de aproximación
        std::vector<cv::Point> approx;
        cv::approxPolyDP(contour, approx, epsilon, true);    // Aproximar a polígono
		
        if (approx.size() == 4 && cv::isContourConvex(approx)) {
            double area = cv::contourArea(approx);
            if (area > 500) { // Filtrar por tamaño mínimo
                rectangle = approx;
                break; // Tomar el primer rectángulo válido
			}
		}
	}
	
    if (rectangle.empty()) {
        std::cerr << "No se encontró un rectángulo en la imagen." << std::endl;
        
	}
	
    // Paso 4: Enderezar el rectángulo usando transformación de perspectiva
    std::vector<cv::Point2f> srcPoints, dstPoints;
    for (const auto& point : rectangle)
	srcPoints.push_back(cv::Point2f(point.x, point.y));
	
    // Destino: un rectángulo alineado
    float width = 400;  // Ancho deseado
    float height = 300; // Alto deseado
    dstPoints = {cv::Point2f(0, 0), cv::Point2f(width, 0),
	cv::Point2f(width, height), cv::Point2f(0, height)};
	
    cv::Mat perspectiveMatrix = cv::getPerspectiveTransform(srcPoints, dstPoints);
    cv::Mat aligned;
    cv::warpPerspective(input, aligned, perspectiveMatrix, cv::Size(width, height));
	
    
    cv::Mat drawing = input.clone();
    cv::polylines(drawing, rectangle, false, cv::Scalar(0, 255, 0,150)	, 10);
	
	return aligned;
    
}

int main(int argc, char* argv[]){
	
	sf::RenderWindow window;
	Mat Imagen;
	if(argc > 1){
		//printf("Sin Implementacion desde Archivo");
		Mat Img_File = imread(argv[1]);
		
		if (Img_File.empty()) {
			cout << "No se pudo cargar la imagen." << endl;
			return -1;
		}
		//cv::resize(Img_File, Img_File, cv::Size(), 0.5f, 0.5f, cv::INTER_LINEAR);
		
		cvtColor(Img_File, Img_File, COLOR_BGR2RGBA);
		Imagen = Img_File;
		
	}else if( argc == 1){	
		Mat Img_Cam = PreproceCamara();
		window.create(sf::VideoMode(Img_Cam.cols, Img_Cam.rows), "Desde Camara");
		Imagen = Img_Cam;
	}
	sf::Texture texture;
	
	Imagen = detectAndAlignRectangle(Imagen);
	cv::flip(Imagen, Imagen, 1);
	texture.create(Imagen.cols, Imagen.rows);
	window.create(sf::VideoMode(Imagen.cols, Imagen.rows), "Desde Archivo");
	//cvtColor(Imagen, Imagen, COLOR_BGR2RGBA);
	texture.update(Imagen.data);
	
	
	
    sf::Sprite sprite(texture);
	
	
	window.setVerticalSyncEnabled(true);
	while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
			}
		}
		
        // Renderizar la imagen
        window.clear();
        window.draw(sprite);
        window.display();
	}
	
	
	
	return 0;
}