
# include <iostream>
# include <string>
# include <vector>
# include <fstream>
# include <sstream>


int main(int argc, char *argv[])
{
    std::string i_file = "";
    std::string o_file = "";
    int k = 0;
    std::vector<int> setA, setB;

    if (argc < 5){
        std::cout << "Please specify input and output files:\nUsage: ./main -i in_file -o out_file" << std::endl;
        exit(1);
    }

    if (argv[1] == "-i"){
        i_file = argv[2];
        o_file = argv[4];
    }else{
        i_file = argv[4];
        o_file = argv[2];
    }

    std::ifstream infile(i_file);

    if (!infile){
        std::cerr << "cant open file" << std::endl;
        exit(1);
    }

    infile >> k;
    
    std::string line;
    std::getline(infile, line);
    std::istringstream ibuff(line);
    
    parse_csv(setA, ibuff);

    line = "";
    std::getline(infile, line);
    ibuff = std::istringstream(line);

    parse_csv(setB, ibuff);
    

    infile.close();



    
    return 0;
}

std::vector<int> parse_csv(std::vector<int> & set, std::istringstream & ibuff){
    int i;
    while (ibuff >> i){
        set.push_back(i);
        
        if (ibuff.peek() == ','){
            ibuff.ignore();
            continue;
        }

        if (ibuff.peek() == '-'){
            ibuff.ignore();
            int upper;
            ibuff >> upper;
            for (int j = i+1; j <= upper; ++upper){
                set.push_back(j);
            }
        }
    }
}
