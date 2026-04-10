using System;
using System.IO;

class Program {
    static void Main(string[] args) {
        string name = args.Length > 0 ? args[0] : "junk";

        string code = $@"
using System;

class Program {{
    static void Main() {{
        Console.WriteLine(""[{name}] does absolutely nothing useful."");
    }}
}}";

        string file = name + ".cs";
        File.WriteAllText(file, code);

        System.Diagnostics.Process.Start("mcs", file).WaitForExit();

        Console.WriteLine("Built: " + name + ".exe");
    }
}
