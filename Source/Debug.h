/*
  ==============================================================================

    Debug.h
    Created: 22 Dec 2020 10:25:11am
    Author:  admin

  ==============================================================================
*/

#pragma once
//
//#ifdef JUCE_DEBUG
//#define DEBUG_PRINT true
//#define DEBUG_LOG false
//#else
//#define DEBUG_PRINT false
//#define DEBUG_LOG false
//#endif
//
//#include <fstream>
//#include <iostream>

// #define watch(x) cout << (#x) << " is " << (x) << endl
//
//
//template <typename Arg, typename... Args>
//void LOG(Arg&& arg, Args&&... args) {
//    std::ofstream outfile;
//    outfile.open("LoudnessRiderLog.txt", std::ios_base::app);
//    outfile << std::forward<Arg>(arg);
//    ((outfile << ',' << ' ' << std::forward<Args>(args)), ...);
//    outfile << std::endl;
//    outfile.close();
//}
//
//template <typename Arg, typename... Args>
//void PRINT(Arg&& arg, Args&&... args) {
//    std::cout << std::forward<Arg>(arg);
//    ((std::cout << ',' << ' ' << std::forward<Args>(args)), ...);
//    std::cout << std::endl;
//}
//
//#ifdef JUCE_DEBUG
//template <typename Arg, typename... Args>
//void // thebug(Arg&& arg, Args&&... args) {
 //   if (DEBUG_LOG)   { LOG(arg, args...); }
   // if (DEBUG_PRINT) { PRINT(arg, args...); }
//}
//#else
////template <typename Arg, typename... Args>
////void // thebug(Arg&& arg, Args&&... args) { }
//#endif
