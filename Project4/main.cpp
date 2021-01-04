#include<iostream>
#include<vector>
#include"AudioProcessor.h"

void TestCompress()
{
    AudioProcessor aProc;

    // INPUT 1
    std::vector<short> input{ 1, 2, 5, 6, 10, 0, -2, -5, -6, -8};
    std::vector<short> expected{1, 2, 5, 5, 7, 0, -2, -5, -5, -6};
    auto output{ aProc.Compress(input, 5, 3) };

    if (output != expected)
        std::cout << "bad Compressor! failed on input 1." << std::endl;

    ///////////
    bool exceptonThrown{ false };
    try
    {
        auto util = aProc.Compress({1,2}, -1, 2);
    }
    catch(std::invalid_argument& e)
    {
        exceptonThrown = true;
    }

    if(!exceptonThrown)
        std::cout << "bad Compressor! threshold<0 didn't throw the exception." << std::endl;

    ///////////
    exceptonThrown = false;
    try
    {
        auto util = aProc.Compress({ 1,2 }, 1, 0.5f);
    }
    catch (std::invalid_argument& e)
    {
        exceptonThrown = true;
    }

    if (!exceptonThrown)
        std::cout << "bad Compressor! rate<1 didn't throw the exception." << std::endl;
}

void TestCutOutSilence()
{
    AudioProcessor aProc;

    // INPUT 1
    std::vector<short> input{ 1, 2, 5, 6, 10, 0, -2, -5, -5, -8 };
    std::vector<short> expected{ 1, 2, 5, 6, 10, -8 };
    auto output{ aProc.CutOutSilence(input, 5, 4) };

    if (output != expected)
        std::cout << "bad CutOutSilence! failed on input 1." << std::endl;

    // INPUT 2
    input = { 1, 2, 5, 6, -4, 4, 10, 0, -2, -5, -4};
    expected = { 6, -4, 4, 10 };
    output = aProc.CutOutSilence(input, 5, 3);

    if (output != expected)
        std::cout << "bad CutOutSilence! failed on input 2." << std::endl;

    // INPUT 3
    input = { 1, 2, 5, 6, -4, 4, -2, -5, 10, 0, -4 };
    expected = { 1, 2, 5, 6, 10, 0, -4 };
    output = aProc.CutOutSilence(input, 5, 4);

    if (output != expected)
        std::cout << "bad CutOutSilence! failed on input 3." << std::endl;

    ///////////
    bool exceptonThrown{ false };
    try
    {
        auto util = aProc.CutOutSilence({ 1,2 }, -1, 2);
    }
    catch (std::invalid_argument& e)
    {
        exceptonThrown = true;
    }

    if (!exceptonThrown)
        std::cout << "bad CutOutSilence! silenceLevel<0 didn't throw the exception." << std::endl;

    ///////////
    exceptonThrown = false;
    try
    {
        auto util = aProc.CutOutSilence({ 1,2 }, 1, 0);
    }
    catch (std::invalid_argument& e)
    {
        exceptonThrown = true;
    }

    if (!exceptonThrown)
        std::cout << "bad CutOutSilence! silenceMinLength<1 didn't throw the exception." << std::endl;
}

void TestStretchTwice()
{
    AudioProcessor aProc;

    // INPUT 1
    std::vector<short> input{ 1, 2, 6, -5, -10, 1 };
    std::vector<short> expected{ 1, 2, 2, 4, 6, 1, -5, -8, -10, -5, 1 };
    auto output{ aProc.StretchTwice(input) };

    if (output != expected)
        std::cout << "bad StretchTwice! failed on input 1." << std::endl;

    //////////
    if (aProc.StretchTwice({}) != std::vector<short>{})
        std::cout << "bad StretchTwice! failed on the empty input std::vector." << std::endl;
    
    //////////
    if (aProc.StretchTwice({4}) != std::vector<short>{4})
        std::cout << "bad StretchTwice! failed on the single-element input std::vector." << std::endl;
}

void TestNormalize()
{
    AudioProcessor aProc;

    // INPUT 1
    std::vector<short> input{ 1, 2, -5, 8 };
    std::vector<short> expected{ 2, 3, -8, 12 };
    auto output{ aProc.Normalize(input, 12) };

    if (output != expected)
        std::cout << "bad Normalize! failed on input 1." << std::endl;

    // INPUT 2
    input = { -10, 2, -5, 8 };
    expected = { -20, 4, -10, 16 };
    output = aProc.Normalize(input, 20);

    if (output != expected)
        std::cout << "bad Normalize! failed on input 2." << std::endl;

    ///////////
    bool exceptonThrown{ false };
    try
    {
        auto util = aProc.Normalize({ 1,2 }, 0);
    }
    catch (std::invalid_argument& e)
    {
        exceptonThrown = true;
    }

    if (!exceptonThrown)
        std::cout << "bad Normalize! normalizeTarget<1 didn't throw the exception." << std::endl;
}

int main()
{
    TestCompress();
    TestCutOutSilence();
    TestStretchTwice();
    TestNormalize();

    return 0;
}