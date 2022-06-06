using System.Collections.Generic;

Console.WriteLine("Running DTE Code Generation");

Console.WriteLine("Generating ECS boilerplate code");
const string ECS_SRC_FOLDER = "../src/ECS/";

var componentFileLines = File.ReadAllLines(ECS_SRC_FOLDER + "component.h");

File.Delete(ECS_SRC_FOLDER + "component_gen.hpp");
var fileStream = File.Create(ECS_SRC_FOLDER + "component_gen.hpp");

List<string> componentNames = new List<string>(); 

foreach (var line in componentFileLines) {
    if(line.Length >= 6 && line.Trim().Substring(0, 6) == "struct") {
        string restOfTheLine = line.Trim().Substring(6);
        restOfTheLine = restOfTheLine.Trim();
        int nextWhitespace = restOfTheLine.IndexOf(' ');
        string componentName = restOfTheLine.Substring(0, nextWhitespace);
        Console.WriteLine ("Found component name: " + componentName);
        componentNames.Add(componentName);
    }
}

using (StreamWriter writer = new StreamWriter(fileStream)) {
    writer.WriteLine("/* this is a generated file. Please dont modify manually */");

    foreach (string componentName in componentNames) {
        writer.WriteLine("struct " + componentName + "Array {");
        writer.WriteLine("\tu32 count;");
        writer.WriteLine("\t" + componentName + " " + componentName.ToLower() + "s [MAX_ENTITIES];");
        writer.WriteLine("};");
    }

    writer.WriteLine("");
    writer.WriteLine("");

    foreach (string componentName in componentNames) {
        writer.WriteLine(componentName + "Array " + componentName +  "s;");
    }
} 

Console.WriteLine("Code sucessfully generated");
