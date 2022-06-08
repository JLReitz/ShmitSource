#include <iostream>

#include <ShmitCore/Types/Containers/Buffer.hpp>

using namespace std;

int main()
{
    shmit::Buffer<int> buf(10);

    int x = 1;
    for (size_t i = 0; i < 13; i++)
    {
        buf.push_back(i);
        buf.PrintDiagnostic();
        cout << endl;
    }

    // cout << "Front of buffer is " << buf.front() << endl;
    // cout << "Back of buffer is " << buf.back() << endl << endl;

    // buf.pop_front();
    // buf.PrintDiagnostic();
    // buf.pop_back();
    // buf.PrintDiagnostic();
    // cout << endl;

    // size_t bufSize = buf.size();
    // size_t insertIndex = buf._M_get_index_relative_to_front(bufSize / 2);
    // cout << "Buffer is of size " << bufSize << endl << endl;
    // cout << "Accessing buffer at index " << insertIndex << endl;
    // shmit::Buffer<int>::iterator it(buf, insertIndex);
    // buf.insert(it, 69);
    // buf.PrintDiagnostic();
    // cout << endl;
    // buf.insert(it, 69);
    // buf.PrintDiagnostic();
    // cout << endl;
    // buf.insert(it, 69);
    // buf.PrintDiagnostic();
    // cout << endl;
    
    shmit::Buffer<int>::const_iterator cit(buf.begin());
    shmit::Buffer<int>::iterator it2 = cit.undo_const();

    cout << endl;
}