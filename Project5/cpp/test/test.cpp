#include <gtest/gtest.h>
#include "../src/metamodel.h"
// Include headers of files to test

#define TOLERANCE 1.0e-9


// TEST(Ising, MagnetizationIsCorrect){
//   // Check magnetization for some state
//   arma::imat A;
//   A << -1 << 1 << -1 << arma::endr
//     <<  1 << 1 << -1 << arma::endr
//     << -1 << 1 << -1;
//   ASSERT_EQ(magnetization(A),-1);
// }

TEST(MetamodelTest, parameters){
    /* The Metamodel is a black box which inner workings we
       don't care about. We only care about its input/output */
    /* Tests that the parameters and their derived values are correct
       using two different parameter files*/

    Metamodel model_oil("mock_data/", "mock_parameters_1.json");
    ASSERT_EQ(model_oil.getXsteps(), 100);
    ASSERT_EQ(model_oil.getTsteps(), 1000);
    ASSERT_EQ(model_oil.getDim(), 1);
    ASSERT_EQ(model_oil.getMethod(), Method::FORWARD_EULER);

    double dt = 1.0d/1000.0d;
    double dx = 1.0d/(2.0d+100.0d);
    double lower, upper;
    ASSERT_FLOAT_EQ(model_oil.getDt(), dt);
    ASSERT_FLOAT_EQ(model_oil.getDx(), dx);
    ASSERT_FLOAT_EQ(model_oil.getAlpha(), dt/(dx*dx));
    std::tie(lower, upper) = model_oil.getBoundaries();
    ASSERT_FLOAT_EQ(lower, 0.0);
    ASSERT_FLOAT_EQ(upper, 1.0);

    // Input condition should all be zero
    for (double x = 0; x <= 1; x+=0.1)
        ASSERT_FLOAT_EQ(model_oil.initialCondition(x), 0.0);

    Metamodel model_crack("mock_data/", "mock_parameters_2.json");
    /* The Metamodel is a black box which inner workings we
       don't care about. We only care about its input/output */
    ASSERT_EQ(model_crack.getXsteps(), 2);
    ASSERT_EQ(model_crack.getTsteps(), 13242);
    ASSERT_EQ(model_crack.getDim(), 2);
    ASSERT_EQ(model_crack.getMethod(), Method::CRANK_NICOLSON);

    // Input condition should sin(πx)
    for (double x = 0; x <= 1; x+=0.1)
        ASSERT_FLOAT_EQ(model_crack.initialCondition(x), sin(pi*x));

    dt = 1.0d/13242.0d;
    dx = 1.0d/(2.0d+2.0d);
    ASSERT_FLOAT_EQ(model_crack.getDt(), dt);
    ASSERT_FLOAT_EQ(model_crack.getDx(), dx);
    ASSERT_FLOAT_EQ(model_crack.getAlpha(), dt/(dx*dx));
    std::tie(lower, upper) = model_crack.getBoundaries();
    ASSERT_FLOAT_EQ(lower, -5);
    ASSERT_FLOAT_EQ(upper, 0.0);
}
