
#include "kullbackLeiblerDivergency.h"

struct Loss{
    
    Loss(Args &args, double &weight);
    
    public:
        double compute_loss(std::vector<MarkovTable> &mkv_table_vector);
    private:
        double w;
        KullbackLeiblerDivergency kl;
        double average_divergence_vector(std::vector<double> &divergence_vector);
};

