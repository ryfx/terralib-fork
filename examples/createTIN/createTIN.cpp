// TestAkima.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>

#include "../../src/terralib/kernel/TeTin.h"
#include "../../src/terralib/kernel/TeAsciiFile.h"
#include "../../src/terralib/kernel/TeGeometryAlgorithms.h"
#include "../../src/terralib/kernel/TeRaster.h"
#include "../../src/terralib/kernel/TeDefines.h"

//Read samples file in ASCII Spring format
void readSamplesFromSPRFile(TeSampleSet& sampleSet, const std::string& fileName)
{
	TeAsciiFile	pFile (fileName);

	while (pFile.isNotAtEOF())
	{
		std::string geoId;
		std::string repType = pFile.readString ();
		pFile.findNewLine();
		if (repType == "POINT3D")
		{
			geoId = pFile.readString ();
			while ( pFile.isNotAtEOF() && geoId != "END" )
			{
				double x = atof ( geoId.c_str() );
				geoId = pFile.readString ();
				if ( pFile.isNotAtEOF() && geoId != "END" )
				{
					double y = atof ( geoId.c_str() );
					geoId = pFile.readString ();
					if ( pFile.isNotAtEOF() && geoId != "END" )
					{
						double z = atof ( geoId.c_str() );
						sampleSet.add ( TeSample(TeCoord2D(x, y), z) );
						geoId = pFile.readString ();
					}
				}
			}
		}
	}
}

//Read contour lines file in ASCII Spring format
void readContoursFromSPRFile (TeContourLineSet& contourSet, const std::string& fileName)
{
	TeAsciiFile	pFile (fileName);
	//cout.precision(6);
	while (pFile.isNotAtEOF())
	{
		std::string geoId;
		std::string repType = pFile.readString ();
		pFile.findNewLine();
		if (repType == "LINE3D")
		{
			geoId = pFile.readString ();
			if (geoId == "HEIGHT")
			{
				geoId = pFile.readString ();
                double z = atof ( geoId.c_str() );
				//cout << z << endl;
				TeLine2D line;
				geoId = pFile.readString ();
				while ( pFile.isNotAtEOF() && geoId != "END" )
				{
					double x = atof ( geoId.c_str() );
					geoId = pFile.readString ();
					if ( pFile.isNotAtEOF() && geoId != "END" )
					{
						double y = atof ( geoId.c_str() );
						line.add ( TeCoord2D(x, y) );
						//cout << x << " " << y << endl;
						geoId = pFile.readString ();
					}
				}
				//TeLineSimplify(TeLine2D& ptlist, double snap, double maxdist)
//				TeLineSimplify(line, 10, 50);
				TeContourLine cLine (line, z);
				contourSet.add(cLine);
			}
		}
	}
}

void writeTriangleEdgesSPRFile (const std::string& name, TeTin& tin)
{
	fstream sprcfile;
	sprcfile.open ( name.c_str(), ios::out );
	sprcfile << "LINES" << "\n";
	sprcfile << "INFO" << "\n";
	sprcfile << "UNITS Metros" << "\n";
	sprcfile << "INFO_END" << "\n";
	sprcfile.precision(10);

	TeTin::TeTinTriangleIterator ti;
	for (ti = tin.triangleBegin(); ti != tin.triangleEnd(); ti++)
	{
		TeCoord2D pt[3];
		tin.trianglePoints(*ti, pt[0], pt[1], pt[2]);
		sprcfile << pt[0].x() << " " << pt[0].y() << " " << "\n";
		sprcfile << pt[1].x() << " " << pt[1].y() << " " << "\n";
		sprcfile << pt[2].x() << " " << pt[2].y() << " " << "\n";
		sprcfile << pt[0].x() << " " << pt[0].y() << " " << "\n";
		sprcfile << "END" << "\n";
	}
	sprcfile << "END" << "\n";
	sprcfile.close ();
}

int main(int argc, char* argv[])
{
//	1. Read samples
//	1.1 Read XYZ samples from SPRING file
	TeSampleSet samples;
	readSamplesFromSPRFile( samples, "..\\data\\samples.spr" );

//	1.2 Read cntour lines from SPRING file
	TeContourLineSet contours;
	readContoursFromSPRFile( contours, "..\\data\\samples.spr"  );

//	2. Create Original TIN
	TeBox tinBox( samples.box() );
	updateBox(tinBox, contours.box() );

	TeTin tt;
	tt.createInitialTriangles (tinBox);

//	2.1. Set precision used for building TIN
	double tol = TePrecision::instance().precision(); // Save old precision
	double tinPrecision = tinBox.width() / 1.e6; // 1.e6th of deltaX
	TePrecision::instance().setPrecision(tinPrecision);

//	2.2. Insert samples in TIN
	TeSampleSet::iterator si;
	for (si = samples.begin(); si != samples.end(); si++)
	{
		double x = (*si).location().x();
		double y = (*si).location().y();
		double z = (*si).value();
		tt.insertPoint (x, y, z);
	}

	TeContourLineSet::iterator ci;
	for (ci = contours.begin(); ci != contours.end(); ci++)
	{
		TeContourLine line ( *ci );

		//TeLineSimplify(TeLine2D& ptlist, double snap, double maxdist)
		TeLineSimplify(line, tinPrecision*400., tinPrecision*2000);

		double z = (*ci).value();
		TeContourLine::iterator li;
		for (li = line.begin(); li != line.end(); li++)
		{
			double x = (*li).x();
			double y = (*li).y();
			tt.insertPoint (x, y, z);
		}
	}


//	2.3. Fix edge triangles TIN
	tt.convexize();

	writeTriangleEdgesSPRFile (string("..\\data\\tin.spr" ), tt);

	TePrecision::instance().setPrecision(tol);

	return 0;
}

