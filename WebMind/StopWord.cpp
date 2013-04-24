///////////////////////////////////////////////////////////////
// StopWord.cpp: implementation of the CStopWord class.
//
// This software was written by Tingshao Zhu.
//
// Copyright (C) 2000-2004 Tingshao Zhu. All rights reserved!
// Use in commercial applications requires written permission
// This software is provided "as is", with no warranty.
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StopWord.h"
#include "Connection.h"
#include "fstream.h"
#include <search.h>
#include <vector>
#include <string>
using namespace std;

// this is the list of stop words

char* stop_words[NUM_STOP_WORDS] = {
  
"%23","%24","%25","%28","%29","%2A","%40","%5F","%60","%7B","%7C","%7D","%7E",
"%A1%A2","%A1%A3","%A1%A4","%A1%A6","%A1%A7","%A1%AC","%A1%AD","%A1%B0","%A1%B1","%A1%B2",
"%A1%B3","%A1%B4","%A1%B5","%A1%B6","%A1%B7","%A1%B8","%A1%B9","%A1%BA","%A1%BB","%A1%BC",
"%A1%BD","%A1%BE","%A1%BF","%A1%C0","%A1%C1","%A1%C2","%A1%C3","%A1%C4","%A1%C5","%A1%C6",
"%A1%C7","%A1%C8","%A1%C9","%A1%CA","%A1%CC","%A1%D4","%A1%D9","%A1%E1","%A1%E2","%A1%FA",
"%A1%FB","%A1%FC","%A1%FD","%A3%A1","%A3%A8","%A3%A9","%A3%AC","%A3%BA","%A3%BB","%A3%BC",
"%A3%BE","%A3%BF","%B0%A1","%B0%A1%B9%FE","%B0%A1%D1%BD","%B0%A1%D3%B4","%B0%A2","%B0%A4%B4%CE",
"%B0%A4%B8%F6","%B0%A4%BC%D2%B0%A4%BB%A7","%B0%A4%C3%C5%B0%A4%BB%A7",
"%B0%A4%C3%C5%D6%F0%BB%A7","%B0%A4%D7%C5","%B0%A5","%B0%A5%D1%BD","%B0%A5%D3%B4","%B0%A6",
"%B0%B3","%B0%B3%C3%C7",
"%B0%B4","%B0%B4%C0%ED","%B0%B4%C6%DA","%B0%B4%CA%B1","%B0%B4%CB%B5","%B0%B4%D5%D5",
"%B0%B5%B5%D8%C0%EF","%B0%B5%D6%D0","%B0%B5%D7%D4","%B0%BA%C8%BB","%B0%C9","%B0%C9%DF%D5",
"%B0%CB","%B0%CB%B3%C9","%B0%CD","%B0%CD%B0%CD","%B0%D1","%B0%D5%C1%CB","%B0%E3%B5%C4","%B1%A8",
"%B1%B3%B5%D8%C0%EF","%B1%B3%BF%BF%B1%B3","%B1%B6%B8%D0","%B1%B6%BC%D3","%B1%BB","%B1%BE","%B1%BE%B1%A8",
"%B1%BE%B5%D8","%B1%BE%C8%CB","%B1%BE%C9%ED","%B1%BE%D7%C5","%B1%C2","%B1%C8","%B1%C8%B7%BD","%B1%C8%BC%B0",
"%B1%C8%C6%F0","%B1%C8%C8%E7","%B1%C8%C8%E7%CB%B5","%B1%C8%D5%D5","%B1%C9%C8%CB","%B1%CB","%B1%CB%B4%CB",
"%B1%CB%CA%B1","%B1%CF%BE%B9","%B1%D8%B6%A8",
"%B1%D8%BD%AB","%B1%D8%D0%EB","%B1%DF","%B1%E3%D3%DA","%B1%E4","%B1%F0",
"%B1%F0%B4%A6","%B1%F0%B5%C4","%B1%F0%B9%DC","%B1%F0%C8%CB","%B1%F0%CA%C7",
"%B1%F0%CB%B5","%B2%A2","%B2%A2%B7%C7","%B2%A2%BC%E7","%B2%A2%C3%BB","%B2%A2%C3%BB%D3%D0",
"%B2%A2%C5%C5","%B2%A2%C7%D2","%B2%A2%CE%DE","%B2%AA%C8%BB","%B2%BB","%B2%BB%B1%C8",
"%B2%BB%B1%D8","%B2%BB%B3%A3","%B2%BB%B3%C9","%B2%BB%B4%F3","%B2%BB%B5%A5","%B2%BB%B5%AB",
"%B2%BB%B5%C3","%B2%BB%B5%C3%B2%BB","%B2%BB%B5%C3%C1%CB","%B2%BB%B5%C3%D2%D1","%B2%BB%B5%FC",
"%B2%BB%B6%A8","%B2%BB%B6%C0","%B2%BB%B6%D4","%B2%BB%B7%C1","%B2%BB%B9%DC","%B2%BB%B9%DC%D4%F5%D1%F9",
"%B2%BB%B9%E2","%B2%BB%B9%FD","%B2%BB%BB%E1","%B2%BB%BD%F6","%B2%BB%BD%F6%BD%F6","%B2%BB%BD%F6%BD%F6%CA%C7",
"%B2%BB%BE%A1","%B2%BB%BE%A1%C8%BB","%B2%BB%BE%AD%D2%E2","%B2%BB%BE%D0","%B2%BB%BF%C9%BF%AA%BD%BB",
"%B2%BB%BF%C9%BF%B9%BE%DC","%B2%BB%C1%A6","%B2%BB%C1%CB","%B2%BB%C1%CF","%B2%BB%C2%DB","%B2%BB%C2%FA",
"%B2%BB%C3%E2","%B2%BB%C4%DC","%B2%BB%C4%DC%B2%BB","%B2%BB%C5%C2","%B2%BB%C6%F0","%B2%BB%C7%C9",
"%B2%BB%C8%BB","%B2%BB%C8%BB%B5%C4%BB%B0","%B2%BB%C8%D5","%B2%BB%C8%E7","%B2%BB%C8%F4","%B2%BB%C9%D9",
"%B2%BB%CA%A4","%B2%BB%CA%B1","%B2%BB%CA%C7","%B2%BB%CC%D8","%B2%BB%CD%AC","%B2%BB%CD%E2",
"%B2%BB%CD%E2%BA%F5","%B2%BB%CE%A9","%B2%BB%CE%CA","%B2%BB%CF%C2","%B2%BB%CF%DE","%B2%BB%CF%FB",
"%B2%BB%D2%AA","%B2%BB%D2%D1","%B2%BB%D2%E0%C0%D6%BA%F5","%B2%BB%D3%C9%B5%C3","%B2%BB%D4%D9",
"%B2%BB%D4%F1%CA%D6%B6%CE","%B2%BB%D4%F5%C3%B4","%B2%BB%D4%F8","%B2%BB%D6%AA%B2%BB%BE%F5",
"%B2%BB%D6%B9","%B2%BB%D6%B9%D2%BB%B4%CE","%B2%BB%D6%BB","%B2%BB%D6%C1%D3%DA","%B2%C5",
"%B2%C5%C4%DC","%B2%DF%C2%D4%B5%D8","%B2%EE%B2%BB%B6%E0","%B2%EE%D2%BB%B5%E3","%B3%A3%D1%D4%B5%C0",
"%B3%A3%D1%D4%CB%B5","%B3%A3%D1%D4%CB%B5%B5%C3%BA%C3","%B3%A4%B4%CB%CF%C2%C8%A5",
"%B3%A4%BB%B0%B6%CC%CB%B5","%B3%A4%C6%DA%D2%D4%C0%B4","%B3%A8%BF%AA%B6%F9","%B3%AF",
"%B3%AF%D7%C5","%B3%B9%D2%B9","%B3%C3","%B3%C3%B1%E3","%B3%C3%BB%FA","%B3%C3%C8%C8","%B3%C3%CA%C6",
"%B3%C3%D4%E7","%B3%C3%D7%C5","%B3%C9%C4%EA%C0%DB%D4%C2","%B3%C9%D0%C4","%B3%CB","%B3%CB%BB%FA",
"%B3%CB%CA%C6","%B3%CB%CF%B6","%B3%CB%D0%E9","%B3%CF","%B3%CF%C8%BB","%B3%CF%C8%E7","%B3%D9%D4%E7",
"%B3%E4%B7%D6","%B3%E4%C6%E4%BC%AB","%B3%E4%C6%E4%C1%BF","%B3%E5","%B3%E9%C0%E4%D7%D3","%B3%F6%C0%B4",
"%B3%F6%C8%A5","%B3%F6%D3%DA","%B3%FD","%B3%FD%B4%CB","%B3%FD%B4%CB%B6%F8%CD%E2","%B3%FD%B4%CB%D2%D4%CD%E2",
"%B3%FD%B4%CB%D6%AE%CD%E2","%B3%FD%B7%C7","%B3%FD%BF%AA","%B3%FD%C1%CB","%B3%FD%C8%A5","%B3%FD%C8%B4",
"%B3%FD%CD%E2","%B4%A6%B4%A6","%B4%A6%D4%DA","%B4%A8%C1%F7%B2%BB%CF%A2","%B4%AB%CB%B5","%B4%AB%CE%C5",
"%B4%BF%B4%E2","%B4%CB","%B4%CB%B4%A6","%B4%CB%B4%CE","%B4%CB%B5%D8","%B4%CB%BA%F3","%B4%CB%BC%E4",
"%B4%CB%CA%B1","%B4%CB%CD%E2","%B4%CB%D6%D0","%B4%CE%B5%DA","%B4%D2%B4%D2","%B4%D3","%B4%D3%B2%BB",
"%B4%D3%B4%CB","%B4%D3%B4%CB%D2%D4%BA%F3","%B4%D3%B6%F8","%B4%D3%B9%C5%B5%BD%BD%F1","%B4%D3%B9%C5%D6%C1%BD%F1",
"%B4%D3%BD%F1%D2%D4%BA%F3","%B4%D3%BF%ED","%B4%D3%C0%B4","%B4%D3%C7%E1","%B4%D3%CB%D9","%B4%D3%CD%B7",
"%B4%D3%CE%B4","%B4%D3%CE%DE%B5%BD%D3%D0","%B4%D3%D0%A1","%B4%D3%D0%C2","%B4%D3%D1%CF","%B4%D3%D3%C5",
"%B4%D3%D4%E7%B5%BD%CD%ED","%B4%D3%D6%D0","%B4%D3%D6%D8","%B4%D5%C7%C9","%B4%E6%D0%C4","%B4%EF%B5%A9",
"%B4%F2","%B4%F2%B4%D3","%B4%F3","%B4%F3%B2%BB%C1%CB","%B4%F3%B4%F3","%B4%F3%B5%D6","%B4%F3%B6%BC",
"%B4%F3%B6%E0","%B4%F3%B7%B2","%B4%F3%B8%C5","%B4%F3%BC%D2","%B4%F3%BE%D9","%B4%F3%C2%D4",
"%B4%F3%C3%E6%B6%F9%C9%CF","%B4%F3%CC%E5%C9%CF","%B4%F3%D4%BC","%B4%F3%D5%C5%C6%EC%B9%C4",
"%B4%F3%D6%C2","%B4%F4%B4%F4%B5%D8","%B4%FD","%B4%FD%B5%BD","%B5%A5%B4%BF","%B5%A5%B5%A5","%B5%AB",
"%B5%AB%B7%B2","%B5%AB%CA%C7","%B5%AB%D4%B8","%B5%AF%D6%B8%D6%AE%BC%E4","%B5%B1","%B5%B1%B3%A1",
"%B5%B1%B5%D8","%B5%B1%B6%F9","%B5%B1%BC%B4","%B5%B1%BF%DA%B6%F9","%B5%B1%C8%BB","%B5%B1%CD%A5",
"%B5%B1%CD%B7","%B5%B1%CF%C2","%B5%B1%D5%E6","%B5%B1%D6%D0","%B5%B1%D7%C5","%B5%B9%B2%BB%C8%E7",
"%B5%B9%B2%BB%C8%E7%CB%B5","%B5%B9%CA%C7","%B5%BD","%B5%BD%B4%A6","%B5%BD%B5%D7","%B5%BD%C1%CB%B6%F9",
"%B5%BD%C4%BF%C7%B0%CE%AA%D6%B9","%B5%BD%CD%B7","%B5%BD%CD%B7%C0%B4","%B5%C3","%B5%C3%C1%CB",
"%B5%C3%C6%F0","%B5%C3%CC%EC%B6%C0%BA%F1","%B5%C4","%B5%C4%BB%B0","%B5%C4%C8%B7","%B5%C8","%B5%C8%B5%BD",
"%B5%C8%B5%C8","%B5%D8","%B5%DA","%B5%DA%B6%FE","%B6%A3%DF%CB","%B6%A5%B6%E0","%B6%AF%B2%BB%B6%AF",
"%B6%AF%E9%FC","%B6%B8%C8%BB","%B6%BC","%B6%C0%D7%D4","%B6%CF%C8%BB","%B6%D4","%B6%D4%B1%C8","%B6%D4%B4%FD",
"%B6%D4%B7%BD","%B6%D4%D3%DA","%B6%D9%CA%B1","%B6%E0","%B6%E0%B4%CE","%B6%E0%B6%E0","%B6%E0%B6%E0%C9%D9%C9%D9",
"%B6%E0%B6%E0%D2%E6%C9%C6","%B6%E0%BF%F7","%B6%E0%C3%B4","%B6%E0%C4%EA%C0%B4","%B6%E0%C4%EA%C7%B0",
"%B6%E0%C9%D9","%B6%F8","%B6%F8%BA%F3","%B6%F8%BF%F6","%B6%F8%C2%DB","%B6%F8%C7%D2","%B6%F8%CA%C7",
"%B6%F8%CD%E2","%B6%F8%D1%D4","%B6%F8%D2%D1","%B6%F8%D3%D6","%B6%F9","%B6%FB","%B6%FB%B5%C8","%B6%FB%B6%FB",
"%B6%FB%BA%F3","%B6%FE","%B6%FE%BB%B0%B2%BB%CB%B5","%B6%FE%BB%B0%C3%BB%CB%B5","%B6%FE%C0%B4","%B7%B2",
"%B7%B2%CA%C7","%B7%B4%B5%B9","%B7%B4%B5%B9%CA%C7","%B7%B4%B6%F8","%B7%B4%B9%FD%C0%B4",
"%B7%B4%B9%FD%C0%B4%CB%B5","%B7%B4%CA%D6","%B7%B4%D6%AE","%B7%B4%D6%AE%D2%E0%C8%BB",
"%B7%B4%D6%AE%D4%F2","%B7%BD%B2%C5","%B7%BD%C4%DC","%B7%C7%B3%A3","%B7%C7%B5%AB","%B7%C7%B5%C3",
"%B7%C7%B6%C0","%B7%C7%CC%D8","%B7%C7%CD%BD","%B7%D6%B1%F0","%B7%D6%C6%DA%B7%D6%C5%FA","%B7%D6%CD%B7",
"%B7%DC%D3%C2","%B7%DF%C8%BB","%B7%E7%D3%EA%CE%DE%D7%E8","%B7%F1%D4%F2","%B8%C2","%B8%C2%B5%C7",
"%B8%C2%B8%C2","%B8%C3","%B8%C3%B5%B1","%B8%CF","%B8%CF%BF%EC","%B8%CF%D4%E7%B2%BB%B8%CF%CD%ED",
"%B8%D2%C7%E9","%B8%D2%D3%DA","%B8%D5%B2%C5","%B8%D5%BA%C3","%B8%D5%C7%C9","%B8%DF%B5%CD",
"%B8%F1%CD%E2","%B8%F4%C8%D5","%B8%F4%D2%B9","%B8%F6","%B8%F6%B1%F0","%B8%F6%C8%CB","%B8%F7",
"%B8%F7%B8%F6","%B8%F7%CA%BD","%B8%F7%CE%BB","%B8%F7%D6%D6","%B8%F7%D7%D4","%B8%F8","%B8%F9%BE%DD",
"%B8%FA","%B8%FC%BC%D3","%B8%FC%BD%F8%D2%BB%B2%BD","%B8%FC%CE%AA","%B9%AB","%B9%AB%C8%BB","%B9%B2%D7%DC",
"%B9%BB%C7%C6%B5%C4","%B9%C3%C7%D2","%B9%CA","%B9%CA%B4%CB","%B9%CA%B6%F8","%B9%CA%D2%E2","%B9%CC%C8%BB",
"%B9%D6%B2%BB%B5%C3","%B9%D8%D3%DA","%B9%DC","%B9%DF%B3%A3","%B9%E2%CA%C7","%B9%E9","%B9%E9%B8%F9%B5%BD%B5%D7",
"%B9%E9%B8%F9%BD%E1%B5%D7","%B9%E9%C6%EB","%B9%FB%C8%BB","%B9%FB%D5%E6","%B9%FD","%B9%FD%D3%DA","%B9%FE",
"%B9%FE%B9%FE","%BA%C1%CE%DE","%BA%C1%CE%DE%B1%A3%C1%F4%B5%D8","%BA%C1%CE%DE%C0%FD%CD%E2","%BA%C3",
"%BA%C3%D4%DA","%BA%C7","%BA%C7%BA%C7","%BA%CD","%BA%CE","%BA%CE%B1%D8","%BA%CE%B3%A2","%BA%CE%B4%A6",
"%BA%CE%B7%C1","%BA%CE%BF%E0","%BA%CE%BF%F6","%BA%CE%C0%D6%B6%F8%B2%BB%CE%AA","%BA%CE%CA%B1","%BA%CE%D0%EB",
"%BA%CE%D2%D4","%BA%CE%D6%B9","%BA%D9","%BA%D9%BA%D9","%BA%DC","%BA%DC%B6%E0","%BA%DC%C9%D9","%BA%DF",
"%BA%DF%E0%A1","%BA%E4%C8%BB","%BA%F3","%BA%F3%C0%B4","%BA%F3%D5%DF","%BA%F4%C0%B2","%BA%F4%DF%EA",
"%BA%F5","%BA%F6%B5%D8","%BA%F6%C8%BB","%BB%A5%CF%E0","%BB%A9","%BB%A9%C0%B2","%BB%B0%CB%B5","%BB%B9",
"%BB%B9%CA%C7","%BB%B9%D2%AA","%BB%B9%D3%D0","%BB%BB%BE%E4%BB%B0%CB%B5","%BB%BB%D1%D4%D6%AE","%BB%D0%C8%BB",
"%BB%E1","%BB%ED%C8%BB","%BB%EE","%BB%EF%CD%AC","%BB%F2","%BB%F2%B6%E0%BB%F2%C9%D9","%BB%F2%CA%C7",
"%BB%F2%D0%ED","%BB%F2%D4%BB","%BB%F2%D4%F2","%BB%F2%D5%DF","%BB%F9%B1%BE","%BB%F9%B1%BE%C9%CF","%BB%F9%D3%DA",
"%BC%AB%B4%F3","%BC%AB%B6%C8","%BC%AB%B6%CB","%BC%AB%C1%A6","%BC%AB%C1%CB","%BC%AB%C6%E4","%BC%AB%CE%AA",
"%BC%B0","%BC%B0%C6%E4","%BC%B0%D6%C1","%BC%B1%B4%D2%B4%D2","%BC%B4","%BC%B4%B1%E3","%BC%B4%BB%F2",
"%BC%B4%BD%AB","%BC%B4%BF%CC","%BC%B4%C1%EE","%BC%B4%C8%E7","%BC%B4%C8%F4","%BC%B4%CA%B9","%BC%B4%CA%C7%CB%B5",
"%BC%B8","%BC%B8%B6%C8","%BC%B8%B7%AC","%BC%B8%BA%F5","%BC%B8%BE%AD","%BC%B8%CA%B1","%BC%BA","%BC%C7%D5%DF","%BC%C8",
"%BC%C8%C8%BB","%BC%C8%CA%C7","%BC%C8%CD%F9","%BC%CC%B6%F8","%BC%CC%BA%F3","%BC%CC%D6%AE","%BC%D3%C9%CF","%BC%D3%D2%D4",
"%BC%D3%D6%AE","%BC%D9%C8%E7","%BC%D9%C8%F4","%BC%D9%CA%B9","%BC%E4%BB%F2","%BC%E6%D6%AE","%BC%F2%B6%F8%D1%D4%D6%AE",
"%BC%F2%D1%D4%D6%AE","%BC%F2%D6%B1","%BC%F8%D3%DA","%BD%AB","%BD%AB%B2%C5","%BD%AB%BD%FC","%BD%AB%D2%AA",
"%BD%BB%BF%DA","%BD%CF","%BD%CF%B1%C8","%BD%CF%CE%AA","%BD%CF%D6%AE","%BD%D0","%BD%D3%C1%AC%B2%BB%B6%CF",
"%BD%D3%CF%C2%C0%B4","%BD%D3%D7%C5","%BD%D4%BF%C9","%BD%D8%C8%BB","%BD%D8%D6%C1","%BD%E1%B9%FB","%BD%E5%D2%D4",
"%BD%E8","%BD%E8%B4%CB","%BD%E8%D2%D4","%BD%E9%D3%DA","%BD%EC%CA%B1","%BD%F1","%BD%F4%BD%D3%D7%C5","%BD%F8","%BD%F8%B6%F8",
"%BD%F8%C0%B4","%BD%F8%C8%A5","%BD%FC%BC%B8%C4%EA%C0%B4","%BD%FC%C0%B4","%BD%FC%C4%EA%C0%B4","%BE%A1","%BE%A1%B9%DC",
"%BE%A1%B9%DC%C8%E7%B4%CB","%BE%A1%BF%C9%C4%DC","%BE%A1%BF%EC","%BE%A1%C1%BF","%BE%A1%C8%BB","%BE%A1%C8%E7%C8%CB%D2%E2",
"%BE%A1%D0%C4%BD%DF%C1%A6","%BE%A1%D0%C4%BE%A1%C1%A6","%BE%A1%D4%E7","%BE%AB%B9%E2","%BE%AD","%BE%AD%B3%A3","%BE%AD%B9%FD",
"%BE%B9%B6%F8","%BE%B9%C8%BB","%BE%BF%BE%B9","%BE%C5","%BE%CD","%BE%CD%B4%CB","%BE%CD%B5%D8","%BE%CD%CA%C7",
"%BE%CD%CA%C7%C1%CB","%BE%CD%CA%C7%CB%B5","%BE%CD%CB%E3","%BE%CD%D2%AA","%BE%D3%C8%BB","%BE%D6%CD%E2","%BE%D9%B7%B2",
"%BE%DD","%BE%DD%B3%C6","%BE%DD%B4%CB","%BE%DD%CA%B5","%BE%DD%CB%B5","%BE%DD%CE%D2%CB%F9%D6%AA","%BE%DD%CF%A4",
"%BE%DF%CC%E5%B5%D8%CB%B5","%BE%DF%CC%E5%CB%B5%C0%B4","%BE%E0","%BE%F6%B2%BB","%BE%F6%B7%C7","%BE%F8%B2%BB","%BE%F8%B6%A5",
"%BE%F8%B6%D4","%BE%F8%B7%C7","%BF%AA%CA%BC","%BF%AA%CD%E2","%BF%B4","%BF%B4%C0%B4","%BF%B4%C6%F0%C0%B4","%BF%B4%C9%CF%C8%A5",
"%BF%B4%D1%F9%D7%D3","%BF%BF","%BF%C8","%BF%C9","%BF%C9%BA%C3","%BF%C9%BC%FB","%BF%C9%C4%DC","%BF%C9%CA%C7","%BF%C9%D2%D4",
"%BF%D6%C5%C2","%BF%EC%D2%AA","%BF%F6%C7%D2","%C0%B2","%C0%B4","%C0%B4%B2%BB%BC%B0","%C0%B4%B5%C3%BC%B0","%C0%B4%BD%B2",
"%C0%B4%BF%B4","%C0%B4%CB%B5","%C0%B4%D7%C5","%C0%B4%D7%D4","%C0%B5%D2%D4","%C0%B9%D1%FC","%C0%CE%C0%CE","%C0%CF%C0%CF%CA%B5%CA%B5",
"%C0%CF%CA%C7","%C0%DB%B4%CE","%C0%DB%C4%EA","%C0%E0%C8%E7","%C0%EB","%C0%ED%B5%B1","%C0%ED%B8%C3","%C0%ED%D3%A6",
"%C0%EF","%C0%FD%C8%E7","%C1%A2%B5%D8","%C1%A2%BF%CC","%C1%A2%C2%ED","%C1%A2%CA%B1","%C1%A8","%C1%AA%F1%C7","%C1%AC",
"%C1%AC%C1%AC","%C1%AC%C8%D5","%C1%AC%C8%D5%C0%B4","%C1%AC%C9%F9","%C1%AC%CD%AC","%C1%AC%F1%C7",
"%C1%BD%D5%DF","%C1%CB","%C1%D9","%C1%D9%B5%BD","%C1%E3","%C1%ED","%C1%ED%B7%BD%C3%E6","%C1%ED%CD%E2","%C1%ED%CF%A4",
"%C1%ED%D0%D0","%C1%ED%D2%BB%B7%BD%C3%E6","%C1%ED%D2%BB%B8%F6","%C1%F9","%C2%B7%BE%AD","%C2%C5%B4%CE",
"%C2%C5%B4%CE%C8%FD%B7%AC","%C2%C5%C2%C5","%C2%C6%C2%C6","%C2%CA%B6%FB","%C2%CA%C8%BB","%C2%D4%BC%D3","%C2%D4%CE%A2",
"%C2%D4%CE%AA","%C2%DB","%C2%DB%CB%B5","%C2%ED%C9%CF","%C2%EF","%C2%F0","%C2%FD%CB%B5","%C2%FE%CB%B5","%C3%B0","%C3%B4",
"%C3%BB%C4%CE%BA%CE","%C3%BB%D3%D0","%C3%BF","%C3%BF%B5%B1","%C3%BF%B7%EA","%C3%BF%C3%BF","%C3%BF%CA%B1%C3%BF%BF%CC",
"%C3%C7","%C3%CD%C8%BB","%C3%CD%C8%BB%BC%E4","%C4%A9%23%23%C4%A9","%C4%AA%B2%BB","%C4%AA%B2%BB%C8%BB","%C4%AA%B7%C7",
"%C4%AA%C8%E7","%C4%AA%C8%F4",
"%C4%AC%C4%AC%B5%D8","%C4%AC%C8%BB",
"%C4%B3","%C4%B3%B8%F6","%C4%B3%C4%B3","%C4%B3%D0%A9","%C4%C3","%C4%C4","%C4%C4%B1%DF","%C4%C4%B6%F9",
"%C4%C4%C0%EF","%C4%C4%C4%EA","%C4%C4%C5%C2",
"%C4%C4%CC%EC","%C4%C4%D0%A9","%C4%C4%D1%F9","%C4%C7","%C4%C7%B0%E3","%C4%C7%B1%DF",
"%C4%C7%B6%F9","%C4%C7%B8%F6","%C4%C7%BB%E1%B6%F9","%C4%C7%C0%EF","%C4%C7%C3%B4",
"%C4%C7%C3%B4%D0%A9","%C4%C7%C3%B4%D1%F9","%C4%C7%C4%A9","%C4%C7%CA%B1","%C4%C7%D0%A9",
"%C4%C7%D1%F9","%C4%CB","%C4%CB%D6%C1","%C4%CB%D6%C1%D3%DA","%C4%D1%B5%C0","%C4%D1%B5%C0%CB%B5",
"%C4%D1%B5%C3","%C4%D1%B9%D6","%C4%D1%CB%B5","%C4%D8","%C4%DA","%C4%DC","%C4%DC%B7%F1","%C4%E3",
"%C4%E3%C3%C7","%C4%E3%CA%C7","%C4%EA","%C4%EA%B8%B4%D2%BB%C4%EA","%C4%FA","%C4%FA%C3%C7","%C4%FA%CA%C7","%C4%FD%C9%F1","%C4%FE","%C4%FE%BF%C9",
"%C4%FE%BF%CF","%C4%FE%D4%B8","%C5%B6","%C5%BB","%C5%BC%B6%F8","%C5%BC%B6%FB",
"%C5%BE%B4%EF","%C5%D4%C8%CB","%C5%DE","%C5%E9","%C5%F6%C7%C9","%C6%A9%C8%E7",
"%C6%A9%D3%F7","%C6%AB%C6%AB","%C6%BD%CB%D8","%C6%BE","%C6%BE%BD%E8","%C6%C8%D3%DA",
"%C6%CB%CD%A8","%C6%DF","%C6%E4","%C6%E4%B4%CE","%C6%E4%B6%FE","%C6%E4%BA%F3","%C6%E4%CA%B5","%C6%E4%CB%FB",
"%C6%E4%CB%FC","%C6%E4%D2%BB","%C6%E4%D3%E0","%C6%E4%D6%D0","%C6%F0","%C6%F0%B3%F5","%C6%F0%BC%FB","%C6%F0%C0%B4",
"%C6%F0%CA%D7","%C6%F0%CD%B7","%C6%F0%CF%C8","%C6%F1%B5%AB","%C6%F1%B7%C7","%C6%F1%D6%B9","%C7%A1%B7%EA",
"%C7%A1%BA%C3","%C7%A1%C7%A1","%C7%A1%C7%A1%CF%E0%B7%B4","%C7%A1%C7%C9","%C7%A1%C8%E7","%C7%A1%CB%C6","%C7%A7",
"%C7%A7%CD%F2%C7%A7%CD%F2","%C7%B0%B4%CB","%C7%B0%BA%F3","%C7%B0%D5%DF","%C7%D0%B2%BB%BF%C9","%C7%D0%C4%AA","%C7%D0%C7%D0",
"%C7%D0%CE%F0","%C7%D2","%C7%D2%B2%BB%CB%B5","%C7%D2%CB%B5","%C7%D7%BF%DA","%C7%D7%C9%ED","%C7%D7%CA%D6","%C7%D7%D1%DB",
"%C7%D7%D7%D4","%C7%EA%BF%CC","%C7%EA%BF%CC%BC%E4","%C7%EA%BF%CC%D6%AE%BC%E4","%C7%EB%CE%F0","%C7%EE%C4%EA%C0%DB%D4%C2",
"%C8%A1%B5%C0","%C8%A5","%C8%A8%CA%B1","%C8%AB%B2%BF","%C8%AB%B6%BC","%C8%AB%C1%A6","%C8%AB%C4%EA","%C8%AB%C8%BB","%C8%AB%C9%ED%D0%C4",
"%C8%AB%CC%E5","%C8%B4","%C8%BB%B6%F8","%C8%BB%BA%F3","%C8%C3","%C8%CB","%C8%CB%BC%D2","%C8%CB%C3%C7","%C8%CB%C8%CB","%C8%CE","%C8%CE%BA%CE",
"%C8%CE%C6%BE","%C8%D4","%C8%D4%BE%C9","%C8%D4%C8%BB","%C8%D5","%C8%D5%B8%B4%D2%BB%C8%D5","%C8%D5%BC%FB",
"%C8%D5%BD%A5","%C8%D5%D2%E6","%C8%D5%D5%E9","%C8%E7","%C8%E7%B3%A3","%C8%E7%B4%CB","%C8%E7%B4%CB%B5%C8%B5%C8",
"%C8%E7%B4%CE","%C8%E7%B9%FB","%C8%E7%BA%CE","%C8%E7%BD%F1","%C8%E7%C6%DA","%C8%E7%C6%E4",
"%C8%E7%C7%B0%CB%F9%CA%F6","%C8%E7%C8%F4","%C8%E7%C9%CF","%C8%E7%C9%CF%CB%F9%CA%F6","%C8%E7%CA%C7","%C8%E7%CD%AC",
"%C8%E7%CF%C2","%C8%F4","%C8%F4%B7%C7","%C8%F4%B7%F2","%C8%F4%B9%FB","%C8%F4%CA%C7","%C8%FD",
"%C8%FD%B7%AC%C1%BD%B4%CE","%C8%FD%B7%AC%CE%E5%B4%CE","%C8%FD%CC%EC%C1%BD%CD%B7","%C9%AA%C9%AA",
"%C9%B3%C9%B3","%C9%B6","%C9%CF","%C9%CF%C0%B4","%C9%CF%C8%A5","%C9%CF%CF%C2","%C9%D0%C7%D2","%C9%E8%BB%F2",
"%C9%E8%C8%F4","%C9%E8%CA%B9","%C9%F5%B6%F8","%C9%F5%BB%F2","%C9%F5%C3%B4","%C9%F5%C7%D2","%C9%F5%D6%C1",
"%C9%F5%D6%C1%D3%DA","%CA%A1%B5%C3","%CA%B1%BA%F2","%CA%B2%C3%B4","%CA%B2%C3%B4%D1%F9","%CA%B9","%CA%B9%B5%C3",
"%CA%BC%B6%F8","%CA%C7","%CA%C7%B5%C4","%CA%C7%D2%D4","%CA%D7%CF%C8","%CA%DC%B5%BD","%CA%EB%C1%CF","%CA%EB%D6%AA",
"%CA%FC%BA%F5","%CA%FC%BC%B8","%CB%AD","%CB%AD%C1%CF","%CB%AD%C8%CB","%CB%AD%D6%AA","%CB%B3","%CB%B3%D7%C5","%CB%B5%C0%B4","%CB%C4","%CB%C6%B5%C4",
"%CB%D2","%CB%E4","%CB%E4%C8%BB","%CB%E4%CB%B5","%CB%E4%D4%F2","%CB%E6","%CB%E6%BA%F3","%CB%E6%CA%B1","%CB%E6%D7%C5","%CB%F9","%CB%F9%D0%D2",
"%CB%F9%D2%D4","%CB%F9%D3%D0","%CB%F9%D4%DA","%CB%FB","%CB%FB%C3%C7","%CB%FB%C8%CB","%CB%FB%CA%C7","%CB%FC","%CB%FC%C3%C7","%CB%FC%CA%C7",
"%CB%FD","%CB%FD%C3%C7","%CB%FD%CA%C7","%CC%C8","%CC%C8%BB%F2","%CC%C8%C8%BB","%CC%C8%C8%F4","%CC%C8%CA%B9","%CC%DA","%CC%E6","%CC%E6%B4%FA",
"%CD%A8%B9%FD","%CD%A8%D1%B6%D4%B1","%CD%AC","%CD%AC%CA%B1","%CD%BC","%CD%BC%C6%AC","%CD%DB","%CD%ED%B1%A8","%CD%F2","%CD%F2%D2%BB",
"%CD%F8","%CD%F9","%CD%FB","%CE%A8%D3%D0","%CE%A9%C6%E4","%CE%AA","%CE%AA%B4%CB","%CE%AA%BA%CE","%CE%AA%C1%CB","%CE%AA%CA%B2%C3%B4",
"%CE%AA%D6%B9","%CE%AA%D7%C5","%CE%B9","%CE%CB","%CE%CB%CE%CB","%CE%D2","%CE%D2%C3%C7","%CE%D2%CA%C7","%CE%D8","%CE%D8%BA%F4","%CE%DA%BA%F5",
"%CE%DE","%CE%DE%C2%DB","%CE%DE%C4%FE","%CE%E3%C4%FE","%CE%E5","%CE%FB","%CF%C2","%CF%C5","%CF%C8","%CF%C8%B2%BB%CF%C8","%CF%E0%B6%D4%B6%F8%D1%D4",
"%CF%F1","%CF%F2","%CF%F2%CA%B9","%CF%F2%D7%C5","%CF%FB%CF%A2","%D0%A1","%D0%A9","%D0%C2%CE%C5","%D0%EA",
"%D0%ED%B6%E0","%D1%B6","%D1%BD","%D1%C9","%D1%D8","%D1%D8%D7%C5","%D1%DB","%D2%AA","%D2%AA%B2%BB",
"%D2%AA%B2%BB%C8%BB","%D2%AA%B2%BB%CA%C7","%D2%AA%C3%B4","%D2%AA%CA%C7","%D2%B2","%D2%B2%B0%D5","%D2%B2%BA%C3",
"%D2%B2%BE%CD%CA%C7%CB%B5","%D2%BB","%D2%BB%B0%E3","%D2%BB%B5%A9","%D2%BB%B7%AC","%D2%BB%B7%BD%C3%E6","%D2%BB%B8%F6","%D2%BB%BA%CE",
"%D2%BB%C0%B4","%D2%BB%C7%D0","%D2%BB%D0%A9","%D2%BB%D1%F9","%D2%BB%D4%F2","%D2%BB%D4%F2%CD%A8%B9%FD","%D2%BB%D6%B1",
"%D2%BB%D7%AA%D1%DB","%D2%C0","%D2%C0%BE%DD","%D2%C0%D5%D5","%D2%D1","%D2%D1%D2%D3","%D2%D3",
"%D2%D3%BA%F5","%D2%D3%D4%D5","%D2%D4","%D2%D4%B1%E3","%D2%D4%B9%CA","%D2%D4%BC%B0","%D2%D4%C0%B4","%D2%D4%C3%E2","%D2%D4%C6%DA","%D2%D4%C9%CF",
"%D2%D4%CE%AA","%D2%D4%D6%C1","%D2%D4%D6%C1%D3%DA","%D2%D4%D6%C2","%D2%D6%BB%F2","%D2%E0","%D2%F2","%D2%F2%B4%CB","%D2%F2%B6%F8","%D2%F2%C1%CB",
"%D2%F2%CE%AA","%D2%F2%D7%C5","%D3%B4","%D3%C3","%D3%C3%C0%B4","%D3%C9","%D3%C9%B4%CB","%D3%C9%B4%CB%BF%C9%BC%FB","%D3%C9%CA%C7","%D3%C9%D3%DA",
"%D3%CC%C7%D2","%D3%D0","%D3%D0%B5%C4","%D3%D0%B5%C4%CA%C7","%D3%D0%B9%D8","%D3%D0%BC%B0","%D3%D0%CA%B1","%D3%D0%D0%A9","%D3%D6",
"%D3%D6%BC%B0","%D3%DA","%D3%DA%CA%C7","%D3%DA%CA%C7%BA%F5","%D3%E0%CD%E2","%D3%EB","%D3%EB%B4%CB%CD%AC%CA%B1","%D3%EB%B7%F1","%D3%EB%C6%E4",
"%D3%EB%C6%E4%CB%B5","%D4%BD%CA%C7","%D4%C2","%D4%C6%B6%FB","%D4%D5","%D4%D9","%D4%D9%B4%CE","%D4%D9%C6%E4%B4%CE",
"%D4%D9%CB%B5","%D4%D9%D3%D0","%D4%D9%D4%F2","%D4%D9%D5%DF","%D4%D9%D5%DF%CB%B5","%D4%DA","%D4%DA%CF%C2","%D4%DA%D3%DA","%D4%DB",
"%D4%DB%C3%C7","%D4%F2","%D4%F2%C9%F5",
"%D4%F4%CB%C0","%D4%F5","%D4%F5%C3%B4","%D4%F5%C3%B4%B0%EC","%D4%F5%C3%B4%D1%F9","%D4%F5%C4%CE","%D4%F5%D1%F9","%D4%F8","%D5%A3%D1%DB","%D5%A6","%D5%D5",
"%D5%D5%D7%C5","%D5%DF","%D5%E2","%D5%E2%B0%E3","%D5%E2%B1%DF","%D5%E2%B6%F9","%D5%E2%B8%F6","%D5%E2%BB%E1%B6%F9",
"%D5%E2%BE%CD%CA%C7%CB%B5","%D5%E2%C0%EF","%D5%E2%C3%B4","%D5%E2%C3%B4%B5%E3%B6%F9","%D5%E2%C3%B4%D0%A9","%D5%E2%C3%B4%D1%F9",
"%D5%E2%CA%B1","%D5%E2%D0%A9","%D5%E2%D1%F9","%D5%E2%D2%BB%C0%B4","%D5%EB%B6%D4","%D5%FD","%D5%FD%C7%C9","%D5%FD%C8%E7","%D5%FD%CA%C7",
"%D5%FD%D6%B5","%D6%A8","%D6%AE","%D6%AE%C0%E0","%D6%AE%CB%F9%D2%D4","%D6%AE%D2%BB","%D6%B1%B5%BD","%D6%BB","%D6%BB%B5%B1","%D6%BB%C5%C2",
"%D6%BB%CA%C7","%D6%BB%CF%DE","%D6%BB%CF%FB","%D6%BB%D2%AA","%D6%BB%D3%D0","%D6%C1",
"%D6%C1%BD%F1","%D6%C1%C8%F4","%D6%C1%D3%DA","%D6%C2","%D6%D6","%D6%EE","%D6%EE%C8%E7","%D6%EE%CE%BB","%D6%F0%B2%BD","%D7%C5","%D7%C5%C4%D8",
"%D7%CA%C1%CF","%D7%D3","%D7%D4","%D7%D4%B4%D3","%D7%D4%B4%F2","%D7%D4%B8%F6","%D7%D4%B8%F6%B6%F9","%D7%D4%B8%F7%B6%F9","%D7%D4%BA%F3",
"%D7%D4%BC%BA","%D7%D4%BC%D2","%D7%D4%C9%ED","%D7%DB%C9%CF%CB%F9%CA%F6","%D7%DC%B5%C4%C0%B4%BF%B4","%D7%DC%B5%C4%C0%B4%CB%B5",
"%D7%DC%B5%C4%CB%B5%C0%B4","%D7%DC%B6%F8%D1%D4%D6%AE","%D7%DC%D6%AE","%D7%DD","%D7%DD%C1%EE","%D7%DD%C8%BB","%D7%DD%CA%B9","%D7%EE",
"%D7%EE%BA%F3","%D7%F1%D1%AD","%D7%F1%D5%D5","%D7%F6","%D7%F7%CE%AA","%D9%CE%C8%BB","%D9%E2","%DF%C0","%DF%C2","%DF%CB","%DF%D6","%DF%D7","%DF%F6",
"%DF%FD","%E0%A5%B5%B1","%E0%B6","%E0%B8%E0%A1","%E0%C0","%E0%C5","%E0%C8","%EC%A3","&","-","-%3E","0","1","2","3","4","5","6","7","8","9","?",


	    "a", "about", "above", "across", "after",
        "again", "against", "all", "almost", "alone",
        "along", "already", "also", "although", "always",
        "among", "an", "and", "another", "any", 
        "anybody", "anyone", "anything", "anywhere", "are",
        "area", "areas", "around", "as", "ask",
        "asked", "asking", "asks", "at", "away",

        "b", "back", "backed", "backing", "backs", 
        "be", "because", "become", "becomes", "became",
        "been", "before", "began", "behind", "being",
        "beings", "best", "better", "between", "big",
        "both", "but", "by", 

        "c", "came", 
        "can", "cannot", "case", "cases", "certain",
        "certainly","charset", "clear", "clearly", "com", "come", "could",

        "d", "did", "differ", "different", "differently",
        "do", "does", "done", "down", "downed", 
        "downing", "downs", "during", 

        "e", "each", "early", "either", "end", "ended", "ending",
        "ends", "enough", "even", "evenly", "ever",
        "every", "everybody", "everyone", "everything", "everywhere", 

        "f", "face", "faces", "fact", "facts",
        "far", "felt", "few", "find", "finds",
        "first", "for", "four", "from", "full",
        "fully", "further", "furthered", "furthering", "furthers", 

        "g", "gave", "general", "generally", "get",
        "gets", "give", "given", "gives", "go",
        "going", "good", "goods", "got", "great",
        "greater", "greatest", "group", "grouped", "grouping", 
        "groups", 

        "h", "had", "has", "have",
        "having", "he", "her", "herself", "here",
        "high", "higher", "highest", "him", "himself",
        "his", "how", "however", "html", "http",

        "i", "if", "important", "in", "interest", "interested", "interesting",
        "interests", "into", "is", "it", "its",
        "itself", 

        "j", "just", 

        "k", "keep", "keeps", "kind", "knew", "know", "known", 
        "knows", 

        "l", "large", "largely", "last",
        "later", "latest", "least", "less", "let",
        "lets", "like", "likely", "long", "longer",
        "longest", 

        "m", "made", "make", "making", 
        "man", "many", "may", "me", "member",
        "members", "men", "might", "more", "most",
        "mostly", "mr", "mrs", "much", "must",
        "my", "myself", 

        "n", "necessary", "need", 
        "needed", "needing", "needs", "never", "new",
        "newer", "newest", "next", "no", "non",
        "not", "nobody", "noone", "nothing", "now",
        "nowhere", "number", "numbered", "numbering", "numbers",

        "o", "of", "off", "often", "old",
        "older", "oldest", "on", "once", "one",
        "only", "open", "opened", "opening", "opens",
        "or", "order", "ordered", "ordering", "orders",
        "other", "others", "our", "out", "over",

        "p", "part", "parted", "parting", "parts",
        "per", "perhaps", "place", "places", "point",
        "pointed", "pointing", "points", "possible", "present",
        "presented", "presenting", "presents" "problem", "problems",
        "put", "puts", 

        "q", "quite", 


        "r", "rather", "really", "right", "room", "rooms",

        "s", "said", "same", "saw", "say",

        "says", "second", "seconds", "see", "sees",

        "seem", "seemed", "seeming", "seems", "several",
        "shall", "she", "should", "show", "showed",
        "showing", "shows", "side", "sides", "since",
        "small", "smaller", "smallest", "so", "some",
        "somebody", "someone", "something", "somewhere", "state",
        "states", "still", "such", "sure", 

        "t", "take", "taken", "than", "that", "the",
        "their", "them", "then", "there", "therefore",
        "these", "they", "thing", "things", "think",
        "thinks", "this", "those", "though", "thought",
        "thoughts", "three", "through", "thus", "to",
        "today", "together", "too", "took", "toward",
        "turn", "turned", "turning", "turns", "two",

        "u", "under", "until", "up", "upon",
        "us", "use", "uses", "used", "utf",

        "v", "very", 

        "w", "want", "wanted", "wanting",
        "wants", "was", "way", "ways", "we",
        "well", "wells", "went", "were", "what",
        "when", "where", "whether", "which", "while",
        "who", "whole", "whose", "why", "will",
        "with", "within", "without", "work", "worked",
        "working", "works", "would", "www",


        "x", 

        "y", "year", "years", "yet", "you", "young",
        "younger", "youngest", "your", "yours", 

        "z"
  
    };

int cmpstopword( char **arg1, char **arg2 )
{
	/* Compare all of both strings: */
	return _strcmpi( *arg1, *arg2 );
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStopWord::CStopWord()
{ /*
	const char filename[] = "D:\\TestUrlEncodes\\stopwordsAfter.txt";
  // ofstream o_file,o_file1;
    ifstream i_file;
    string out_text;
    string temp;
    // read file
    i_file.open(filename);

    if (i_file.is_open())
    {
      while (i_file.good())
        {
           i_file >>temp; //Store the input to out_text
		   out_text+=temp;
		  }
		  
    }

    else
        cout << "Open file " << filename << " error!"; 
    i_file.close();

	vector<string> output=StringSpilt(out_text,',');	
	for(int cc=0;cc<output.length();cc++)
	{
	stopwords[cc]=output[cc];
	}
	//o_file.open("D:\\TestEncodeStopWords\\stopwordsNew.txt");
	//	o_file <<out_text;
//	o_file.close();
*/
}

CStopWord::~CStopWord()
{
}


bool CStopWord::isStopWord(const char* word)
{
  
   char **result;
   CString input(word);

    //WCHAR * unicode=word;

  // char *input;
   //strcpy(unicode,word);
   //if(unicode!=NULL)
	input=Connection::urlEncoding(input);
   
   // Find the word using a binary search algorithm
   result = (char **)bsearch((char *)&input, (char *)stop_words, NUM_STOP_WORDS,
                              sizeof( char * ), (int (*)(const void*, const void*))cmpstopword );
   if( result )
	   return true;
   else
	   return false;

	/*
	// binary search
	int numStop;
    int low, high, mid;

	numStop = numStopWords;

    low = 0;
    high = numStop - 1;

    while (low <= high)
    {
        mid = (low + high) / 2;

        if (stricmp(word,stopwords[mid]) < 0)
            high = mid - 1;
        else if (stricmp(word,stopwords[mid]) > 0)
            low = mid + 1;
        else
            return true;
    }
    
    return false;
	*/
}
