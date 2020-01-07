#ifndef PADDING_H
#define PADDING_H


namespace padding {

    class FixedRecord {

    public:

        static unsigned int calculPaddingAfterEachElements(unsigned int &offset, const unsigned long &sizeElement, const unsigned int &V) {
            unsigned int P = 0;
            //Calcul padding after each element which depends on the encoded lenght of the element
            unsigned int R = (offset+sizeElement)%V;
            //R = (sizeElement+P)&(V-1); //Use this code to replace the modulo calculation by mask instead classic modulo
            P = (R == 0) ? 0:(V-R);
            offset += (sizeElement + P);

            return P;
        }

    };

    class VariantRecord {

    public:

        static unsigned int calculPaddingAfterDiscriminant(const unsigned long &sizeDiscriminent, const unsigned int &V) {

            //Calcul padding after nbElements indication
            unsigned int P = 0;
            unsigned int R = (sizeDiscriminent+P)%V;
        //    R = (S+P)&(V-1); //Use this code to replace the modulo calculation by mask intead classic modulo
            P = (R == 0) ? 0:(V-R);

            return P;
        }

    };

    class HlaArray {
    public:

        static unsigned int calculPaddingAfterNbElements(const unsigned int &octetBoundary) {
            unsigned int P = 0;
            unsigned int V = std::max(octetBoundary, 4u);
            unsigned int R = (4+P)%V;
        //    R = (4+P)&(V-1); //Use this code to replace the modulo calculation by mask intead classic modulo
            P = (R == 0) ? 0:(V-R);

            return P;
        }

        static unsigned int calculPaddingAfterEachElements(const unsigned long &sizeElement, const unsigned int &octetBoundary) {
            unsigned int V = std::max(octetBoundary, 4u);
            unsigned int P = 0;
            unsigned int R = (sizeElement+P)%V;
        //    R = (sizeElement+P)&(V-1); //Use this code to replace the modulo calculation by mask intead classic modulo
            P = (R == 0) ? 0:(V-R);
            return P;
        }
    };

}

#endif // PADDING_H
