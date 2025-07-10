import subprocess

def run_echo(input_data):
    process = subprocess.Popen(
        ['main.exe'],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )
    stdout, stderr = process.communicate(input_data)
    return stdout.strip(), stderr.strip()


def test_echo():
    tests = [
        {
            "input": "2*3*4\n",
            "expected_output": "24\n0"
        },
        {
            "input": "10 +  5 -   3\n",
            "expected_output": "12\n0"
        },
        {
            "input": "20/2/2\n",
            "expected_output": "5\n0"
        },

        {
            "input": "min(min(1234,5678),min(2345,6789)) * 50 - 2300 / 100\n",
            "expected_output": "61677\n0"
        },
        {
            "input": "min( min( 1234 , 5678 ) , min( 2345 , 6789 ) ) * 50 - 2300 / 100\n",
            "expected_output": "61677\n0"
        },
        {
            "input": "   5   +   3   \n",
            "expected_output": "8\n0"
        },

        {
            "input": "(-1) * ((-2) - (-3))\n",
            "expected_output": "-1\n0"
        },

        {
            "input": "def Add(x, y, z) { x * z + y }\nAdd(1,2,3)\n",
            "expected_output": "0\n5\n0"
        },

        {
            "input": "def super_duper(x, y) { min(abs(max(-x,-y)),pow(x,y)) }\nsuper_duper(1, 2)\n",
            "expected_output": "0\n1\n0"
        },

        {
            "input": "var result = 5 + 3 * 2\nresult\n",
            "expected_output": "0\n11\n0"
        },
        {
            "input": "var num = 6\ndef double(x) { x * 2 }\ndouble(num)\n",
            "expected_output": "0\n0\n12\n0"
        },

        {
            "input": "def square(n) { n * n }\ndef squareThenAdd(x, y) { square(x) + y }\nsquareThenAdd(4, 10)\n",
            "expected_output": "0\n0\n26\n0"
        },

        {
            "input": "var a = 5\nvar b = 10\na + b\n",
            "expected_output": "0\n0\n15\n0"
        },
        {
            "input": "def divide(x, y) { x / y }\ndivide(20, 4)\n",
            "expected_output": "0\n5\n0"
        },

        {
            "input": "var global = 100\ndef useGlobal(x) { x + global }\nuseGlobal(5)\n",
            "expected_output": "0\n0\n105\n0"
        }

    ]

    for test in tests:
        input_data = test["input"]
        expected_output = test["expected_output"]

        output, error = run_echo(input_data)

        if error:
            print(f"Test failed for input:\n{input_data}")
            print(f"Error:\n{error}")
        elif output.strip() != expected_output:
            print(f"Test failed for input:\n{input_data}")
            print(f"Expected:\n{expected_output}")
            print(f"Got:\n{output}")
        else:
            print(f"Test passed for input:\n{input_data}")




if __name__ == "__main__":
    test_echo()