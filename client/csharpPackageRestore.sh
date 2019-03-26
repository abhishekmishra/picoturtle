#!/bin/bash

cd ./client/csharp

nuget restore cs-pico.sln -MSBuildVersion 14

cd ../..

ls -al ./client/csharp/packages