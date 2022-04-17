# Cpp soup
This is a library containing some useful functionality. The only dependency is [Catch2](https://github.com/catchorg/Catch2).

## Getting and building
```bash
git clone https://github.com/amartya00/cppsoup.git
cd cppsoup
mkdir _build && cd _build
meson ..
ninja
ninja test
sudo ninja install
```

The usage is explained in the docs. To build the docs, navigate to the root folder `cppsoup` and run the following command:
```bash
doxygen .doxyfile
```
The HTML will be created in a `html/` folder. Start by opening the `index.html` file and navigate around.