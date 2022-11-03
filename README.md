# Computer Vision Project -- Depth Extraction Algorithm with OpenCV
[![Software: C++](https://img.shields.io/badge/Software-C%2B%2B-blue)](http://www.cplusplus.org/)  [![Library: OpenCV](https://img.shields.io/badge/Library-OpenCV-green)](https://www.opencv.org/) [![License: MIT](https://img.shields.io/badge/License-MIT-lightgrey)](https://mit-license.org/) 

Developer: Baturay Özgürün

Copyright © 2018 Baturay Özgürün

If you have any requests or questions please do not hesitate to contact me via baturay.ozgurun@gmail.com

---

## Project Description

This algorithm aims at estimating depth maps from stereo images by capitalizing on SIFT features. It also presents the feature coordinates of each stereo image and their best matches in text files.

| **Input 1** | **Input 2** | **Output 1** | **Output 2** | **Output 3** | **Output 4** |
| :---: | :---: | :---: | :---: | :---: | :---: |
| ![left](https://user-images.githubusercontent.com/76174639/199703387-f05e3ed7-1ecf-4ac2-945c-e79178866f8a.jpg) Left image | ![right](https://user-images.githubusercontent.com/76174639/199703400-4525d525-310a-4f0a-a744-855c58f381d4.jpg) Right Image | ![SIFT Features on the Left Image](https://user-images.githubusercontent.com/76174639/199703631-c088c70c-6445-4e06-b17b-75fb47272a99.jpg) SIFT features on the left image | ![SIFT Features on the Right Image](https://user-images.githubusercontent.com/76174639/199703663-f90105ef-99cc-4f2c-9378-6542fdd4a3f5.jpg) SIFT features on the right image | ![Features with the Matches of Left and Right Images](https://user-images.githubusercontent.com/76174639/199704237-746c626a-10cc-4d48-b801-5640cf40b680.jpg) SIFT features with the matches | ![Features with the Best Matches of Left and Right Images](https://user-images.githubusercontent.com/76174639/199704244-b87e3ec2-3946-4f9c-9767-ecb477c4ec9c.jpg) SIFT features with the best matches | 
| **Output 5** | **Output 6** | **Output 7** | **Output 8** | **Output 9** | **Output 10** |
| ![Rectified Left Image](https://user-images.githubusercontent.com/76174639/199704583-31aa1671-3d05-4e4d-a497-417b91a0861e.jpg) Rectified left image | ![Rectified Right Image](https://user-images.githubusercontent.com/76174639/199704595-8d9f9a92-d903-4580-a7c2-1943c66b16d5.jpg) Rectified right Image | ![Disparity Map](https://user-images.githubusercontent.com/76174639/199704612-443ae2c3-8fcf-450d-9d84-9df239a75779.jpg) Disparity map | ![Colorful Disparity Map](https://user-images.githubusercontent.com/76174639/199704622-1affa21f-24fb-4206-b4b1-e7549f27f903.jpg) Disparity map in color | ![The Vector Field on the Original Right Image](https://user-images.githubusercontent.com/76174639/199704637-7189609e-5356-432f-9d4e-ee47f5ed66e3.jpg) Vector field shown on the right image | ![The Vector Field on the Blank Image](https://user-images.githubusercontent.com/76174639/199704648-f7214370-c219-41a8-aaa2-8150714620d0.jpg) Vector field on the blank image |

---

## Software Requirements

The project was developed on macOS 10.13.6 environment. 

In order to run the project, it is required to install the following programs:
- Xcode 10.1
- OpenCV 3.4.0

---

## Installation

### Step 1: Install Xcode
First, it is required to download and install **Xcode** to run OpenCV projects for C++ on macOS environment. To install Xcode go to **App Store** -> **Search bar** -> **Type Xcode** -> **Click Install App** (see the following figure). Make sure that Xcode is stored in the Applications folder. The OpenCV project presented here was developed on **Xcode 10.1** and **macOS 10.13.6**.

![fig1](https://user-images.githubusercontent.com/76174639/197363657-32774932-cf69-407a-b30e-f058c19f1d61.png)

### Step 2: Download OpenCV Package
To download and install **OpenCV 3.4.0** package go to www.opencv.org and then go to **Library** -> **Releases** -> **Download OpenCV 3.4.0 Sources**. Open the zip file and place it on Desktop. In the opencv-3.4.0 folder, create a new folder and name it *build* (see the following figure).

![fig2](https://user-images.githubusercontent.com/76174639/197363690-788fce3a-d926-4c46-87b5-00680dc9ab35.png)

### Step 3: Install OpenCV
Go to www.cmake.org then click Download button. The binary distributions for MacOS are listed on this page (see the following figure). Since the platform is macOS 10.13, download and install **make-3.25.0-rc2-macos-universal.dmg**. Make sure that CMake is stored in the Applications folder.

![fig3](https://user-images.githubusercontent.com/76174639/197363696-e8b0fe57-9071-481e-abee-883fa1ca9bfe.png)

After installing the CMake software, open it and select the source and build folders as shown in the following figure. Then press **Configure** (please see the following figure).

![fig4](https://user-images.githubusercontent.com/76174639/197363701-1b20712b-bb4a-4c84-b821-d9477b289b8d.png)

Select **Unix Makefiles** and **use default native compilers**. Then press **Done**, and wait until the configuration is done (please see the following figure).

![fig5](https://user-images.githubusercontent.com/76174639/197363740-df8a5e2c-2088-4e64-87c8-b6facad92787.png)

After the configuration is done, check the build files out, and then press **Generate** in the CMake software (please see the following figure). 

![fig6](https://user-images.githubusercontent.com/76174639/197363705-6d8c75e5-53fc-4fc1-a0dd-26f731515349.png)

When the generation is done, close the CMake software (please see the following figure). 

![fig7](https://user-images.githubusercontent.com/76174639/197363706-f4f97304-9c75-4844-93d4-9a3aaeb92dbc.png)

Open the terminal, and type the following lines of codes.

``` 
$ cd Desktop/opencv-3.4.0/build
$ make
$ sudo make install
```

### Step 4: Link the OpenCV Libraries to Xcode

After installing OpenCV, open the Xcode project (the file you downloaded; main.xcodeproj). Go to **Build Settings**, and type **paths** in the search bar. Check if **Header Search Paths** and **Library Search Paths** are indicated as shown in the following figure. 

![fig8](https://user-images.githubusercontent.com/76174639/197363708-677a6925-52eb-44e3-99b1-441271f40751.png)

Make sure that the OpenCV libraries are linked to Xcode, and do not forget that the full path name of your libraries might be different than those in the downloaded Xcode project. Therefore, type **linking** in the search bar, and then clear contents in **Other Linker Flags**. Hold and drag the libraries from **Desktop/opencv-3.4.0/build/lib**, and then drop them to **Other Linker Flags** (please see the following figure).

![fig9](https://user-images.githubusercontent.com/76174639/197363714-ab300d14-0022-4c13-9fc9-ffeca5c789a1.png)

### Step 5: Change the Working Directory in Xcode

It is required to define a custom working directory to read and write images in Xcode. To do this, in Xcode, go to **Product** -> **Scheme** -> **Edit Scheme**. Then go to **Options**, and define 'your' **Working Directory**. It should contain the path name of the *main* folder you downloaded. In my case, it is /Users/BATURAY_OZGURUN/Desktop/main (please see the following figure).

![fig10](https://user-images.githubusercontent.com/76174639/197363718-feaa4af1-9e7c-4512-919a-1422d431dd3a.png)

Now the OpenCV project is ready to run.

#### Remarks: Setting Up OpenCV Extra Modules

This project may require an extra module, e.g., xfeatures2d. To build the extra module, go to https://github.com/opencv/opencv_contrib and download **opencv_contrib-3.4** folder. In this folder, find the module (e.g., xfeatures2d), copy and paste it into the **modules** folder of opencv-3.4.0. Open the CMake software, and then repeat Step 3 and Step 4.

---

## Citation

If you use this project in your research, please cite it as follows:

B. Ozgurun, Computer Vision Project -- Depth Extraction Algorithm with OpenCV, (2022), GitHub repository, https://github.com/baturayozgurun/Computer_Vision_Project_Depth_Extraction_Algorithm_with_OpenCV

---

## License

This project is licensed under [MIT License](https://mit-license.org/).
