/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright (C) 2013-2020 Synaptics Incorporated. All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner
 * unless Synaptics has otherwise provided express, written
 * permission.
 *
 * Use of the materials may require a license of intellectual property
 * from a third party or from Synaptics. This file conveys no express
 * or implied licenses to any intellectual property rights belonging
 * to Synaptics.
 *
 * INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS", AND
 * SYNAPTICS EXPRESSLY DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES,
 * INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY
 * INTELLECTUAL PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION WITH THE USE
 * OF THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER CAUSED AND
 * BASED ON ANY THEORY OF LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, AND EVEN IF SYNAPTICS WAS
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. IF A TRIBUNAL OF
 * COMPETENT JURISDICTION DOES NOT PERMIT THE DISCLAIMER OF DIRECT
 * DAMAGES OR ANY OTHER DAMAGES, SYNAPTICS' TOTAL CUMULATIVE LIABILITY
 * TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
 */

#include "synap/file_utils.hpp"
#include "synap/logging.hpp"

#include <fstream>
#include <iostream>

using namespace std;

namespace synaptics {
namespace synap {

bool file_exists(const string& file_name)
{
    ifstream f(file_name.c_str());
    return f.good();
}


string filename_extension(const string& file_name)
{
    size_t i = file_name.rfind('.', file_name.length());
    if (i != string::npos) {
        return (file_name.substr(i + 1, file_name.length() - i));
    }

    return "";
}

string filename_without_extension(const string& file_name)
{
    size_t i = file_name.rfind('/', file_name.length());
    string filename_nopath;
    if (i != string::npos) {
        // Remove path
        filename_nopath = file_name.substr(i + 1, file_name.length() - i);
    }
    else {
        filename_nopath = file_name;
    }
    i = filename_nopath.rfind('.', filename_nopath.length());
    if (i != string::npos) {
        // Remove extension
        return (filename_nopath.substr(0, i));
    }

    return file_name;
}

std::string filename_path(const std::string& file_name)
{
    size_t i = file_name.rfind('/', file_name.length());
    if (i == string::npos) {
        return "";
    }
    return file_name.substr(0, i + 1);
}


string file_read(const string& file_name)
{
    ifstream f(file_name);
    if (!f.good()) {
        cerr << "Error: can't read input file: " << file_name << endl;
        return {};
    }
    string file_content(istreambuf_iterator<char>{f}, {});
    return file_content;
}


vector<uint8_t> binary_file_read(const string& file_name)
{
    ifstream f(file_name, ios::binary);
    if (!f.good()) {
        cerr << "Error: can't read input file: " << file_name << endl;
        return {};
    }
    vector<uint8_t> file_content(istreambuf_iterator<char>{f}, {});
    return file_content;
}


bool binary_file_write(const string& file_name, const void* data, size_t size)
{
    ofstream wf;
    wf.open(file_name, ios::out | ios::binary);
    if (data) {
        wf.write(static_cast<const char*>(data), size);
    }
    wf.close();
    return wf.good();
}

}  // namespace synap
}  // namespace synaptics
