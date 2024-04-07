#include "population.h"

Individual Population::getBest() const{
    Individual best = population[0];
    for (Individual ind : population){
        if (ind < best){
            best = ind;
        }
    }
    return best;
}

std::vector<std::string> Population::getSelectionTypes() const {
    return selectionList;
}
std::vector<std::string> Population::getCrossoverTypes() const{
    return crossoverList;
}
std::vector<std::string> Population::getMutationTypes() const {
    return mutationList;
}

void Population::setSelectionType(const std::string &type) {
    if(std::find(selectionList.begin(), selectionList.end(), type) != selectionList.end()){
        selectionType = type;
    } else {
        // add error with reference to getSelectionTypes();
    }
}
void Population::setCrossoverType(const std::string &type){
    if(std::find(crossoverList.begin(), crossoverList.end(), type) != crossoverList.end()){
        crossoverType = type;
    } else {
        // add error with reference to getCrossoverTypes();
    }
}
void Population::setMutationTypes(const std::string &type){
    if(std::find(mutationList.begin(), mutationList.end(), type) != mutationList.end()){
        mutationType = type;
    } else {
        // add error with reference to getMutationTypes();
    }
}