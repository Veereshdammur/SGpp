/*
 * regressionByInterpolation.cpp
 *
 *  Created on: Dec 17, 2015
 *      Author: pfandedd
 */

#include <fstream>

#include <sgpp/base/datatypes/DataVector.hpp>
#include <sgpp/base/grid/Grid.hpp>
#include <sgpp/base/grid/GridStorage.hpp>
#include <sgpp/base/grid/generation/GridGenerator.hpp>
#include <sgpp/base/operation/hash/OperationEval.hpp>
#include <sgpp/base/operation/BaseOpFactory.hpp>

#include <sgpp/datadriven/operation/hash/OperationOcttreeHistogramRegression/OperationOcttreeHistogramRegression.hpp>

using namespace SGPP::base;
using namespace std;

// function to reconstruct
SGPP::float_t f(std::vector<float_t> point) {
    return 16.0 * (point[0] - 1) * point[0] * (point[1] - 1) * point[1];
}

// function to reconstruct
SGPP::float_t f(SGPP::base::DataVector point) {
    return 16.0 * (point[0] - 1) * point[0] * (point[1] - 1) * point[1];
}

int main(int argc, char **argv) {

    size_t dim = 2;
    size_t samplePoints = 200;

    DataMatrix dataset(0, dim);
    DataVector values(samplePoints);

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    std::ofstream sampleFile;
    sampleFile.open("sampleFile.csv");

    for (size_t sample = 0; sample < samplePoints; sample++) {
        vector<double> point(dim);
        for (size_t d = 0; d < dim; d++) {
            point[d] = dist(mt);
            sampleFile << point[d] << ", ";
        }
        dataset.appendRow(point);
        values[sample] = f(point);
        sampleFile << values[sample] << std::endl;
    }
    sampleFile.close();

    SGPP::datadriven::OperationOcttreeHistogramRegression piecewiseRegressor(dataset, values);

    std::unique_ptr<SGPP::datadriven::HistogramTree::Node> node = piecewiseRegressor.hierarchize(0.001, 10);

//    std::ofstream resultFile;
//    resultFile.open("resultFile.csv");
//
//    for (size_t sample = 0; sample < samplePoints; sample++) {
//        vector<double> point(dim);
//        for (size_t d = 0; d < dim; d++) {
//            point[d] = dist(mt);
//            resultFile << point[d] << ", ";
//        }
//        double eval = node->evaluate(point);
//        resultFile << eval << std::endl;
//    }
//    resultFile.close();

    std::ofstream resultFile;
    resultFile.open("resultFile.csv");

    for (SGPP::float_t testX = 0; testX <= 1.0; testX += 0.01f) {
        for (SGPP::float_t testY = 0; testY <= 1.0; testY += 0.01f) {
            vector<SGPP::float_t> point = { testX, testY };
            for (size_t d = 0; d < dim; d++) {
                resultFile << point[d] << ", ";
            }
            SGPP::float_t eval = node->evaluate(point);
            resultFile << eval << std::endl;
        }
        resultFile << std::endl;
    }
    resultFile.close();

    std::cout << "all done!" << std::endl;
    return 0;
}

